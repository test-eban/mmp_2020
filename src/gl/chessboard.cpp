#include "chessboard.h"
#include "shaderdebugger.h"
#include "gldisc.h"


//#define DEBUG_CHESSBOARD

ChessBoard::ChessBoard(const QVector3D & center, GLfloat squareSize)
    :GLBody("Chessboard")
{
    m_center = center;
    m_normal = v_Y;
    m_squareSize = squareSize;
    m_radius = m_squareSize * 8.0f * 1.142f;
    setDrawingMode(GL_TRIANGLES);
}
/**
  * Returns 3D position of field center. Field coordinates are from 0 to 7
  */
QVector3D ChessBoard::fieldToPosition(QPoint field)
{
//    QMatrix4x4 m;
//    m.rotate(90.0, QVector3D::crossProduct( m_normal, v_X));
    QVector3D result;
    QVector3D vx = v_X * m_squareSize;
    QVector3D vz = v_Z * m_squareSize;
    result = (field.x() - 3.5f) * vx + (field.y() - 3.5f) * vz;
#ifdef DEBUG_CHESSBOARD
    ShaderDebugger::debugVector3D(result, "Field" + QString::number(field.x()) +"," + QString::number(field.y())
                       + " is at position ");
#endif
    return result;
}

void ChessBoard::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);
    m_firstPoint = pointsSize();
    m_firstIndex = indicesSize();

    QVector3D vx = v_X * m_squareSize;
   // vx.debugOutput("ChessBoard::vx: ");
    QVector3D vz = v_Z * m_squareSize;
   // vz.debugOutput("ChessBoard::vz: ");
    for(GLushort row = 0; row <= 8; row ++){
        QVector3D rowStart = m_center + (row - 4) * vz - 4  * vx;
        for (GLushort column = 0; column <= 8; column ++)
        {
          QVector3D vertex = rowStart + column * vx;
          m_points->append(GLPoint(vertex, m_normal));
          if((row < 8) && (column < 8) && (row + column) % 2 == 0)
          {
              m_indices->append(m_firstPoint + row * 9 + column);
              m_indices->append(m_firstPoint + (row + 1) * 9 + column);
              m_indices->append(m_firstPoint + row * 9 + column + 1);
              m_indices->append(m_firstPoint + row * 9 + column + 1);
              m_indices->append(m_firstPoint + (row + 1) * 9 + column);
              m_indices->append(m_firstPoint + (row + 1) * 9 + column + 1);
          }
        }
    }
    m_nextPoint = pointsSize();
    m_nextIndex = indicesSize();

    /*
    QString s;
    for(int i = m_firstIndex; i < m_nextIndex; i++)
    {
        s += QString::number(indices.at(i)) + " ";
    }
    qDebug("indices: %s", qPrintable(s));
     */
}

/**
  * Raise disc so that it jumps over field.
  */
void ChessBoard::raiseDisc(GLDisc * disc, QPoint field, bool jumpOverKing)
{
    GLfloat distance = (fieldToPosition(field) - QVector3D(disc->getCenter().x(), 0.0, disc->getCenter().z())).length();
    GLfloat radius = 1.41f * m_squareSize;
    GLfloat raiseHeight = 3.0f * disc->getHeight() * (1.0f - distance * distance / radius / radius);
    if(jumpOverKing)
        raiseHeight *= 2.0f;
    QVector3D oldCenter = disc->getCenter();
    GLfloat oldHeight = QVector3D::dotProduct(oldCenter, m_normal);
    if(raiseHeight < 0.0f)
        raiseHeight = 0.0f; //do not dive
    QVector3D vRaise = m_normal * (raiseHeight - oldHeight);
    QMatrix4x4 m;
    m.translate(oldCenter + vRaise);
    disc->setTransformation(m);
}

/**
  * Returns true, if position is on board.
  */
bool ChessBoard::isOnBoard(const QVector3D & position)
{
    return
            position.x() < m_center.x() + 4.0f * m_squareSize
        &&  position.x() > m_center.x() - 4.0f * m_squareSize
        &&  position.z() < m_center.z() + 4.0f * m_squareSize
        &&  position.z() > m_center.z() - 4.0f * m_squareSize;
}
