#include "glcylinder.h"
#include <math.h>

GLCylinder::GLCylinder(const QString & name, float height, float radius, const GLColorRgba &color, const QString textureFile, int slices)
    :GLBody(name, radius, color, textureFile)
{
 m_height = height;
 m_slices = slices;
 m_center = v_Zero;
 m_drawingMode = GL_TRIANGLE_STRIP;
}

void GLCylinder::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);
    QVector3D topCenter = v_Z * m_height;
    QVector3D bottomCenter = m_center;
    QVector3D vertex;
    QVector3D cylinderNormal;
    QVector3D texCoord;
    QMatrix4x4 longitudeMatrix;
    IndexType iBottomCenter = 0; //indices for the poles
    IndexType iTopCenter = 0;

    //start with south pole
    m_firstPoint = static_cast<IndexType>(m_points->size());
    m_firstIndex = static_cast<IndexType>(m_indices->size());
    iBottomCenter = m_firstPoint;
    m_points->append(GLPoint(bottomCenter,            //vertex
                           -v_Z,                      //normal
                           QVector3D(0.125, 0.5, 0.0),//texcoord of left circle center
                           m_color));

    IndexType firstRow = 1;
    IndexType lastRow = 4;
    IndexType slice;
    for(slice = 0; slice <= m_slices; slice ++)
    {
        longitudeMatrix.setToIdentity();//always start from scratch
        longitudeMatrix.rotate(slice * 360.0f / m_slices, -v_Z);
        float barrelIncrement = 0.5f / m_slices;
        //First point is for bottom circle
       QVector3D unitVertex = v_X * longitudeMatrix;
       vertex = v_X * longitudeMatrix;
       cylinderNormal = vertex;
       texCoord = QVector3D(0.125f + 0.125f * unitVertex.x(), 0.5f + 0.5f * unitVertex.y(), 0.0f); //edge of left circle
       vertex *= m_radius;
       m_points->append(GLPoint(vertex, -v_Z, texCoord, m_color));

       //second point is for bottom cylinder side
       float texX = 0.25f + barrelIncrement * slice;
        texCoord = QVector3D(texX, 0.0, 0.0);//bottom edge of side rect
       m_points->append(GLPoint(vertex, cylinderNormal, texCoord, m_color));

       //third point is for top cylinder side
       vertex += topCenter;
       texCoord = QVector3D(texX, 1.0, 0.0) ;//top edge of side rect
       m_points->append(GLPoint(vertex, cylinderNormal, texCoord, m_color));

       //fourth point is for top circle
       texCoord = QVector3D(0.875f + 0.125f * unitVertex.x(), 0.5f + 0.5f * unitVertex.y(), 0.0f) ; //edge of right circle
       m_points->append(GLPoint(vertex, v_Z, texCoord, m_color));
    }
    //end with top center
    iTopCenter =  static_cast<IndexType>(m_points->size());
    m_points->append(GLPoint(topCenter,               //vertex
                           v_Z,                       //normal
                           QVector3D(0.875, 0.5, 0.0),//texcoord of right circle center
                           m_color));
    m_nextPoint = static_cast<IndexType>(m_points->size());

    IndexType rows = 4;
    for(slice = 0; slice < m_slices; slice += 2)
    {
        //go upwards from bottom center to top center
        m_indices->append(iBottomCenter);
        for(IndexType stack = firstRow; stack <= lastRow; stack ++)
        {
           m_indices->append(iBottomCenter + rows * slice + rows + stack ); //right meridian
           m_indices->append(iBottomCenter + rows * slice + stack); //left meridian
        }
        m_indices->append(iTopCenter);

        //now go downwards back to bottomCenter
        for(IndexType stack = lastRow; stack >= firstRow; stack --)
        {
           m_indices->append(iBottomCenter + rows * slice + rows + rows + stack); //right meridian
           m_indices->append(iBottomCenter + rows * slice + rows + stack); //left meridian
        }
        //bottomCenter will be added in next slice
    }

    //Close the barrel
    //go upwards from south pole to north pole
    m_indices->append(iBottomCenter);
    for(IndexType stack = firstRow; stack <= lastRow; stack ++)
    {
       m_indices->append(iBottomCenter + stack ); //right meridian = first meridian
       m_indices->append(iBottomCenter + rows * slice + stack); //left meridian = last meridian
    }
    m_indices->append(iTopCenter);

    m_indices->append(iBottomCenter); //finally add south pole

    m_nextIndex = static_cast<IndexType>(m_indices->size());

    if(m_showNormals)
        createNormals( 2.0f * m_radius);

    //
}

