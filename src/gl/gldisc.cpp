#include "gldisc.h"
#include "math.h"

GLDisc::GLDisc(GLColorRgba color, QPoint fieldCoord, GLfloat radius, GLfloat height, int slices)
    :GLBody("Disc", radius, color)
{
    m_fieldCoord = fieldCoord;
    m_height = height;
    m_slices = slices;
    m_type = Standard;
    setShowFrame(true);
}


void GLDisc::makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);
    QVector3D vCenter = m_center;
    QVector3D vRadius = v_X * m_radius;
    QVector3D vUp = v_Y * m_height;
    if(m_type == King)
        vUp = 2.0 * vUp;
    QMatrix4x4 m;
    m.rotate(360.0f / m_slices, v_Y );
    QVector3D vNextRadius = m * vRadius;
    m_firstPoint = pointsSize();
    //make the surface as a series of pie pieces
    for(int i = 0; i <= m_slices; i++)
    {
        //bottom triangle, face downwards
        m_points->append(GLPoint (vCenter, -v_Y));
        m_points->append(GLPoint (vCenter + vNextRadius, -v_Y));
        m_points->append(GLPoint (vCenter + vRadius, -v_Y));

        //side triangles
        m_points->append(GLPoint(vCenter + vRadius, vRadius.normalized()));
        m_points->append(GLPoint(vCenter + vNextRadius, vNextRadius.normalized()));
        m_points->append(GLPoint(vCenter + vRadius + vUp, vRadius.normalized()));

        m_points->append(GLPoint(vCenter + vNextRadius, vNextRadius.normalized()));
        m_points->append(GLPoint(vCenter + vNextRadius + vUp, vNextRadius.normalized()));
        m_points->append(GLPoint(vCenter + vRadius + vUp, vRadius.normalized()));

        //top triangle, face upwards
        m_points->append(GLPoint (vCenter + vUp, v_Y));
        m_points->append(GLPoint (vCenter + vRadius + vUp, v_Y));
        m_points->append(GLPoint (vCenter + vNextRadius + vUp, v_Y));

        vRadius = vNextRadius;
        if(i < m_slices - 1){
            m.setToIdentity();
            m.rotate((i + 1) * 360.0f / m_slices, v_Y);
            vNextRadius = m * (v_X * m_radius);
        }
        else vNextRadius = (v_X * m_radius); //last point = first point
    }
    m_nextPoint = pointsSize();
    findMinMaxCoordinates();
}

void GLDisc::calculateDrawMatrix()
{
  m_drawMatrix = m_transformationMatrix;
  m_drawMatrix.translate(m_liftVector);
}

void GLDisc::draw(GLESRenderer *renderer, bool useBuffers)
{
    GLBody::draw(renderer,useBuffers);
    if(isKing()){
        renderer->pushMvMatrix();
        renderer->translate(v_Y * 2.0f);
        GLBody::draw(renderer,useBuffers);
        renderer->popMvMatrix();
    }
}

/**
  * Returns true, if GLDiscs are colliding
  */
bool GLDisc::isColliding(const GLDisc * other)
{
    if(this == other)
        return false;

    if((m_center - other->m_center).length() > m_radius + other->m_radius)
        return false;
    else
    {
        if(fabs(m_center.y() - other->m_center.y()) >((m_height + other->m_height) / 2.0f))
            return false;
        else return true;
    }
}

/**
  * Set type to King
  */
void GLDisc::becomeKing()
{
    m_type = King;
}

void GLDisc::jumpUp()
{
    m_finalLiftVector = v_Y*3;
    m_startLiftVector = v_Zero;
}

void GLDisc::jumpDown()
{
    m_finalLiftVector = v_Zero;
    m_startLiftVector = m_liftVector;
}

void GLDisc::finishAnimation()
{
    m_liftVector = m_finalLiftVector;
}

void GLDisc::updateAnimatedProperties(float animationState)
{
   m_liftVector = m_startLiftVector + localAnimationState(animationState) *(m_finalLiftVector - m_startLiftVector);
}
