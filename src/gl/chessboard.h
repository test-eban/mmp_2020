/****************************************************************************
 *                     Copyright by Walter Roth 2012                        *
 *                       Licence: GPL                                       *
 ****************************************************************************/
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "glbody.h"
#include <QPoint>

class GLDisc;
/**
  * A chess board with 8X8 fields in the x-z plane.
  * Only the white fields are drawn, black fields are transparent.
  * The raiseDisc function makes a disc "jump" over the respective field.
  * The fieldToPosition function returns the 3D-coordinates of the field in model space.
  * isOnBoard returns true, when position is "on" the board.
  */
class ChessBoard : public GLBody
{
public:
    ChessBoard(const QVector3D & center = v_Zero, GLfloat squareSize = 1.0);
    /**
      * Returns 3D position of field center. Field coordinates are from 0 to 7
      */
    QVector3D fieldToPosition(QPoint field);


    GLfloat getSquareSize(){return m_squareSize;}
    /**
      * Raise disc so that it jumps over field.
      */
    void raiseDisc(GLDisc * disc, QPoint field, bool jumpOverKing);

    /**
      * Returns true, if position is on board.
      */
    bool isOnBoard(const QVector3D & position);
    void makeSurface(QVector<GLPoint> * pointContainer = nullptr, QVector<GLushort> *indexContainer = nullptr);
protected:
    /**
      * The normal on the chessboard plane
      */
    QVector3D m_normal;
    /**
      * The size of the black and white squares
      */
    GLfloat m_squareSize;
};

#endif // CHESSBOARD_H
