#include "glcube.h"

GLCube::GLCube(const QString & name, const QVector3D & lbb, const QVector3D & rtf, const GLColorRgba & color,  const QString & textureFile )
    :GLBody(name, (rtf - lbb).length() / 2.0f, color, textureFile)
{
  m_lbb = lbb;
  m_rtf = rtf;
}

void GLCube::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);
    createCube(m_lbb, m_rtf);
}

void GLCube::createCube(QVector3D lbb, QVector3D rtf)
{
    /**
          Corner vertices of the cube

          ltb_____________rtb
            /            /|
           /            / |
       ltf/____________/rtf
          |   |        |  |
          |   |        |  |
          |lbb|________|__|rbb
          |  /         |  /
          | /          | /
       lbf|/___________|/rbf

          */
    m_firstPoint = static_cast<GLushort>( m_points->size());
    m_firstIndex = indicesSize();
    //corners
    //QVector3D lbb = lbb;
    QVector3D rbb = QVector3D(rtf.x(), lbb.y(), lbb.z());
    QVector3D ltb = QVector3D(lbb.x(), rtf.y(), lbb.z());
    QVector3D lbf = QVector3D(lbb.x(), lbb.y(), rtf.z());
    QVector3D rtb = QVector3D(rtf.x(), rtf.y(), lbb.z());
    QVector3D rbf = QVector3D(rtf.x(), lbb.y(), rtf.z());
    QVector3D ltf = QVector3D(lbb.x(), rtf.y(), rtf.z());
    //QVector3D rtf = rtf;

    //front
    QVector3D normal = v_Z;
    QVector3D texCoord = v_Zero;
    GLColorRgba color = GLColorRgba::clMagenta;
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, normal, texCoord, color));

    //back
    normal = -v_Z;
    color = GLColorRgba::clRed;
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, normal, texCoord, color));

    //left
    normal = -v_X;
    color = GLColorRgba::clYellow;
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, normal, texCoord, color));

    //right
    normal = v_X;
    color = GLColorRgba::clBlue;
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtb, normal, texCoord, color));

    //top
    normal = v_Y;
    color = GLColorRgba::clGreen;
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, normal, texCoord, color));

    //bottom
    normal = -v_Y;
    color = GLColorRgba::clGreen;
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, normal, texCoord, color));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbf, normal, texCoord, color));

    m_nextPoint= static_cast<GLushort>( m_points->size());
    m_nextIndex = indicesSize();
    //createNormals();

}
