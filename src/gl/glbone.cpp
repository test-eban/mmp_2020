#include "glbone.h"
#include <math.h>
#include "glsphere.h"

GLBone::GLBone(const QString &name, const QVector3D startPoint,
           const QVector3D endPoint, BoneType type , float thickness, GLBody *model, bool makeDebugBodies)
    : GLBody(name)
{
    m_endPoint = endPoint;
    init(startPoint, type, thickness, model, makeDebugBodies);
}

GLBone::GLBone(const QString &name, const QVector3D &modelStartPoint, const QVector3D &modelEndPoint, const QVector3D &modelRotationAxis, float modelRotation,
               const QVector3D &startPoint, GLBone::BoneType type, float thickness, GLBody *model, bool makeDebugBodies)
    :GLBody(name)
{
 QMatrix4x4 rotor;
 rotor.rotate(modelRotation, modelRotationAxis);
 QVector3D modelAxis = modelEndPoint - modelStartPoint;
 QVector3D boneAxis = rotor * modelAxis;
 m_endPoint = startPoint + boneAxis;
 init(startPoint, type, thickness, model, makeDebugBodies);
}

void GLBone::init(const QVector3D startPoint, BoneType type , float thickness, GLBody *model, bool makeDebugBodies){
    m_parent = nullptr;
    m_startPoint = startPoint;
    m_finalStartPoint = startPoint;
    m_transformedStartPoint = m_startPoint;
    m_transformedEndPoint = m_endPoint;
    m_axis = (m_endPoint - m_startPoint).normalized();
    m_center = 0.5f * (m_endPoint + m_startPoint);
    m_length = m_startPoint.distanceToPoint(m_endPoint);
    m_crankAngle = 0.0;
    m_finalCrankAngle = 0.0;
    m_type = type;
    m_makeDebugBodies = makeDebugBodies;
    m_makeTrackLines = false;
    m_ownTracks = true;
    m_selectedTrack = 0;
    m_trackSegment = 0;
    m_slices = 16;
    m_startSphere = nullptr;
    m_endSphere = nullptr;
    m_cylinder = nullptr;
    float length = (m_endPoint - m_startPoint).length();
    float radius = thickness;
    if(thickness == 0.0f)
      radius = length / 10.0f;
    m_startSphere = nullptr;
    m_cylinder = nullptr;
    m_endSphere = nullptr;
    if(model)
    {
        m_ownModel = false;
        m_model = model;
    }
    else{
        m_ownModel = true;
        m_model = new GLBody(GLBody::name() +"_Model");
    }
    if(m_makeDebugBodies)
    {
        m_startSphere = new GLSphere("StartSphere", radius, cl_Red, "", m_slices * 2, m_slices);
        m_cylinder = new GLCylinder("BoneCylinder", length, radius / 2.0f, cl_Blue);
        m_endSphere = new GLSphere("EndShpere", radius, cl_Yellow, "", m_slices * 2, m_slices);
    }
}

GLBone::~GLBone()
{
    if(m_ownModel)
       delete  m_model;
    if(m_startSphere)
        delete m_startSphere;
    if(m_cylinder)
        delete m_cylinder;
    if(m_endSphere)
        delete m_endSphere;
    if(m_ownTracks){
        for(int i = 0; i < m_tracks.size(); i++)
            delete m_tracks[i];
    }
}

void GLBone::destroyTextureObjects()
{
    GLBody::destroyTextureObjects();
    if(m_ownModel)
        m_model->destroyTextureObjects();
}

void GLBone::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);

    QVector3D axis = m_endPoint - m_startPoint;
    float angle = 0.0;
    QVector3D rotAxis = v_X;
    if(fabsf(QVector3D::dotProduct(axis, v_X)) > 0.0001f         //axis is not parallel to Z-axis
            || fabsf(QVector3D::dotProduct(axis, v_Y)) > 0.0001f)
    { //we need to rotate the cylinder
      rotAxis = QVector3D::crossProduct( axis, v_Z);
      angle = -(180.0f / M_PIf) * acosf(QVector3D::dotProduct(axis.normalized(), v_Z));
    }
    else if(m_endPoint.z() < m_startPoint.z()) //end is left of start, we need to turn 180degree
    {
       rotAxis = v_X;
       angle = 180.0f;
    }
    m_transformationMatrix.setToIdentity();
    m_transformationMatrix.translate(-m_startPoint);
    m_transformationMatrix.rotate(-angle, rotAxis);

    if(m_makeDebugBodies)
    {
        m_startSphere->makeSurface(m_points, m_indices);
        m_cylinder->makeSurface(m_points, m_indices);
        m_endSphere->makeSurface(m_points, m_indices);

        QMatrix4x4 m;
        m.translate(m_startPoint);
        m.rotate(angle, rotAxis);
        m_startSphere->setTransformation(m);
        m_cylinder->setTransformation(m);

        m.setToIdentity();
        m.translate(m_endPoint);
        m.rotate(angle, rotAxis);
        m_endSphere->setTransformation(m);
    }
    if(m_makeTrackLines)
        createTrackLines();
}

bool GLBone::setTextureFile(const QString &textureFile)
{
    if(m_ownModel)
        return m_model->setTextureFile(textureFile);
    else return false;
}

bool GLBone::setTextureFiles(const QStringList &textureFiles)
{
    if(m_ownModel)
       return m_model->setTextureFiles(textureFiles);
    else return false;
}

void GLBone::calculateDrawMatrix()
{
     m_drawMatrix.setToIdentity();
     m_drawMatrix.translate(m_translation);
     m_transformedStartPoint = m_startPoint + m_translation;

     if(m_type == BONETYPE_CONROD)
     {
         if(m_tracks.size() == 0)
         {
            // qDebug() << "GLBone::calculateDrawMatrix: Error, no endpoint tracks!";
             return;
         }
         if(m_selectedTrack < 0 || m_tracks.at(m_selectedTrack)->size() < 2)
                return;

         m_trackSegment = 0;
         QVector<QVector3D> intersections;
         //Search first track section with intersection(s)
         do{
             m_trackSegment++;
             intersections = lineSphereIntersection(m_transformedStartPoint, m_length,
                                                   m_tracks.at(m_selectedTrack)->transformedTrackPoint(m_trackSegment - 1),
                                                   m_tracks.at(m_selectedTrack)->transformedTrackPoint(m_trackSegment));

         }while(intersections.size() == 0 && m_trackSegment < m_tracks.at(m_selectedTrack)->size() - 1);
         QVector3D axis1 = v_Y;
         QVector3D axis2 = v_Y;
         QVector3D rotationAxis;
         float angle1 = 400.0;
         float angle2 = 400.0;

         switch(intersections.size())
         {
         case 1:{
                     axis1 = (intersections[0] - m_transformedStartPoint).normalized();
                     angle1 = (180.0f / M_PIf) * acosf(QVector3D::dotProduct(m_axis, axis1));
                 }break;

         case 2:{
                     axis2 = (intersections[1] - m_transformedStartPoint).normalized();
                     angle2 = (180.0f / M_PIf) * acosf(QVector3D::dotProduct(m_axis, axis2));
                 }break;
         default: angle1 = 0.0; //we have no intersection, do not rotate
         }
         //now we perform translation and rotation
         if(angle1 < angle2)
         {
            rotationAxis = QVector3D::crossProduct(m_axis, axis1);
            m_drawMatrix.rotate(angle1, rotationAxis);
         }
         else{
            rotationAxis = QVector3D::crossProduct(m_axis, axis2);
            m_drawMatrix.rotate(angle2, rotationAxis);
         }
     }
     else if(m_type == BONETYPE_CRANK){
         m_drawMatrix.rotate(m_crankAngle, m_crankAxis);
     }
     m_drawMatrix.translate(-m_startPoint);
     QMatrix4x4 shifter;
     shifter.translate(m_startPoint);
     m_drawMatrix = shifter * m_drawMatrix;
     m_transformedEndPoint = m_drawMatrix * m_endPoint;
}

void GLBone::moveStartPointTo(const QVector3D &newStartPoint)
{
    m_translation = newStartPoint - m_startPoint;
    calculateDrawMatrix();
}

int GLBone::addTrack(Track *track)
{
    m_tracks.append(track);
    selectTrack(m_tracks.size() - 1);
    return m_tracks.size() - 1;
}

int GLBone::selectTrack(int trackIndex)
{
    if(trackIndex >= 0 && trackIndex < m_tracks.size())
    {
       m_selectedTrack = trackIndex;
       calculateDrawMatrix();
       return m_selectedTrack;
    }
    else return -1;
}

void GLBone::drawTracks(GLESRenderer *renderer)
{
    int stride = sizeof(GLPoint);
    renderer->setAmbientAndDiffuseColor(cl_White);
    bool oldLightingEnabled = renderer->isLightingEnabled();
    renderer->setLightingEnabled(false);
    bool oldTextureEnabled = renderer->isTextureEnabled();

    //enable required arrays
    renderer->activateAttributeArray(GLESRenderer::VERTEX_LOCATION,
                                     (*m_points)[0].vertexPointer(), stride);
    if (renderer->isLightingEnabled())
        renderer->activateAttributeArray(GLESRenderer::NORMAL_LOCATION,
                                        (*m_points)[0].normalPointer(), stride);
    renderer->setTextureEnabled(false);
    renderer->activateAttributeArray(GLESRenderer::TEXCOORD_LOCATION,
                 (*m_points)[0].texCoordPointer(), stride);
    renderer->activateAttributeArray(GLESRenderer::COLOR_LOCATION,
                 (*m_points)[0].colorPointer(), stride);

    for(int track = 0; track < m_tracks.size(); track ++)
    {
      renderer->pushMvMatrix();
      if(m_tracks.at(track)->transformation() != nullptr)
          renderer->addTransformation(*(m_tracks.at(track)->transformation()));
      glDrawElements(GL_LINE_STRIP, m_nextTrackIndices[track + 1] - m_nextTrackIndices[track],
                     GL_UNSIGNED_SHORT, &m_indices->data()[m_nextTrackIndices[track]]);
      renderer->popMvMatrix();
    }

    //cleanup
    renderer->disableAttributeArrays();
    renderer->setLightingEnabled(oldLightingEnabled);
    renderer->setTextureEnabled(oldTextureEnabled);
}

QVector<QVector3D> GLBone::lineSphereIntersection(const QVector3D &center, float radius, const QVector3D &p1, const QVector3D &p2)
{
    QVector<QVector3D> result;
    QVector3D direction = (p2 - p1).normalized();//direction of line
    float pointDistance = (p2 - p1).length();
    QVector3D p1C = p1 - center; //vector from center to p1
    float p1C2 = QVector3D::dotProduct(p1C, p1C); // p1C length squared
    float dirP1C = QVector3D::dotProduct(direction, p1C);
    float rootValue = dirP1C * dirP1C + radius * radius - p1C2;
    if(rootValue < 0.0f)
        return result; //We have no intersection
    float root =sqrtf(rootValue);
    float lambda1 = -dirP1C + root;
    float lambda2 = -dirP1C - root;

    //Check whether the calculated intersections are between p1 and p2
    if(lambda1 > 0.0f && lambda1 < pointDistance)
      result.append(QVector3D(p1 + lambda1 * direction));
    if(lambda2 > 0.0f && lambda2 < pointDistance)
      result.append(QVector3D(p1 + lambda2 * direction));
    return result;
}

void GLBone::createTrackLines()
{
    if(!m_indices)
    {
        qDebug() << "GLBone::createTrackLines() called without valid index container!";
        return;
    }
    for(int nTrack = 0; nTrack < m_tracks.size(); nTrack++)
    {
       Track * track = m_tracks.at(nTrack);
       m_nextTrackIndices.append(m_indices->size());
       QVector3D trackPoint;
       foreach(trackPoint, *track)
       {
          m_indices->append(static_cast<IndexType>(m_points->size()));
          m_points->append(GLPoint(trackPoint));
       }
    }
    m_nextTrackIndices.append(m_indices->size());
    m_visibleTracks.resize(m_tracks.size());
    for(int i = 0; i < m_visibleTracks.size(); i++)
        m_visibleTracks[i] = true;
}

void GLBone::setCrankAngle(float angle)
{
   m_crankAngle = angle;
   calculateDrawMatrix();
}

void GLBone::setParent(GLBone *parent)
{
    m_parent = parent;
    moveStartPointTo(m_parent->transformedEndPoint());
}

bool GLBone::readBinaryModelFile(const QString &dataFileName, QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    QString fileName;
    foreach (fileName, m_textureFilenames) {
        m_model->setTextureFile(fileName);
    }
    return m_model->readBinaryModelFile(dataFileName, pointContainer, indexContainer);
}

void GLBone::showTrack(int trackNumber)
{
    if(m_visibleTracks.size() == 0)
        createTrackLines();
    if(m_tracks.size() <= trackNumber)
        return;
    if(m_visibleTracks.size() == 0)
    {
        m_visibleTracks.resize(m_tracks.size());
        for(int i = 0; i < m_visibleTracks.size(); i++)
           m_visibleTracks[i] = false;
    }
    m_visibleTracks[trackNumber] = true;
}
void GLBone::hideTrack(int trackNumber)
{
    if(m_tracks.size() <= trackNumber)
        return;
    else m_visibleTracks[trackNumber] = false;
}

void GLBone::setModelTransformation(const QMatrix4x4 m, int i)
{
    if(i < 0)
        m_modelTransformations.append(m);
    else if(i < m_modelTransformations.size())
        m_modelTransformations[i] = m;
    else return;
}

void GLBone::startAnimation(float startValue)
{
    GLBody::startAnimation(startValue);
    m_crankDistance = m_finalCrankAngle - m_crankAngle;
    m_translateDistance = m_finalStartPoint - m_startPoint;
}

void GLBone::finishAnimation()
{
    if(m_type == BONETYPE_CRANK)
      setCrankAngle(m_crankAngle = m_finalCrankAngle);
    if(m_type == BONETYPE_CONROD){
        if(m_parent)//if we have a parent, we simply stick to its endPoint
           moveStartPointTo(m_parent->transformedEndPoint());
      else moveStartPointTo(m_finalStartPoint);
    }
}

void GLBone::updateAnimatedProperties(float animationState)
{
    if(m_parent)//if we have a parent, we simply stick to its endPoint
       moveStartPointTo(m_parent->transformedEndPoint());
    if(m_type == BONETYPE_CRANK)
      setCrankAngle(m_finalCrankAngle - (1.0f - localAnimationState(animationState))* m_crankDistance);
    if(m_type == BONETYPE_CONROD && m_parent == nullptr)//we have no parent and run on our own
        moveStartPointTo(m_finalStartPoint - (1.0f - localAnimationState(animationState))* m_translateDistance);
}

void GLBone::setShowFrame(bool show)
{
    m_model->setShowFrame(show); //frame for model
    GLBody::setShowFrame(show);  //frame for debug geometry
}

void GLBone::draw(GLESRenderer *renderer, bool useBuffers)
{
    if(!m_surfaceIsValid)
      makeSurface(nullptr, nullptr);
    if((m_visibleTracks.size() > 0) && (m_nextTrackIndices.size() == 0)) //we need to create track lines
        createTrackLines();
    calculateDrawMatrix();
    renderer->pushMvMatrix();
    renderer->addTransformation(m_drawMatrix);
    if(m_startSphere)
        m_startSphere->draw(renderer, useBuffers);
    if(m_cylinder)
        m_cylinder->draw(renderer, useBuffers);
    if(m_endSphere)
        m_endSphere->draw(renderer, useBuffers);
    if(m_modelTransformations.size() == 0)
        m_model->draw(renderer,useBuffers);
    else for(int i = 0; i < m_modelTransformations.size(); i++)
    {
        renderer->pushMvMatrix();
        renderer->addTransformation(m_modelTransformations[i]);
        m_model->draw(renderer, useBuffers);
        renderer->popMvMatrix();
    }
    //we want no transformation for the tracks
    renderer->popMvMatrix();
    if(m_nextTrackIndices.size() > 0)
        drawTracks(renderer);
}

void GLBone::setMinMaxCoordinates(const QVector3D min, const QVector3D max)
{
    GLBody::setMinMaxCoordinates(min, max);
    m_model->setMinMaxCoordinates(min, max);
}

