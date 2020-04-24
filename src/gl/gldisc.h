/****************************************************************************
 *                     Copyright by Walter Roth 2012                        *
 *                       Licence: GPL                                       *
 ****************************************************************************/
#ifndef DISC_H
#define DISC_H

#include "glbody.h"
#include "glcolorrgba.h"

#include <QPoint>

/**
  * A simple disc for a Draughts or Checkers game.
  * Use becomeKing to double its height.
  * Use isColliding to detect collisions with other discs.
  */
class GLDisc : public GLBody
{
    typedef enum{
            Standard,
            King
    }DiscType;
public:
    GLDisc(GLColorRgba color, QPoint fieldCoord, GLfloat radius = 0.5f, GLfloat height = 0.25f, int slices = 32);
    /**
      * Create the disc surface.
      */
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer)Q_DECL_OVERRIDE;
    /**
     * @brief draw draw the disc. Draw it twice, if it is a King
     * @param renderer
     * @param useBuffers
     */
    void draw(GLESRenderer * renderer, bool useBuffers = false)Q_DECL_OVERRIDE;

    //simple setters
    void setFieldCoord(const QPoint & newVal){m_fieldCoord = newVal;}

    //simple getters
    const QPoint & getFieldCoord()const{return m_fieldCoord;}
    GLfloat getHeight(){return m_height;}

    /**
      * Returns true, if discs are colliding
      */
    bool isColliding(const GLDisc * other);
    /**
      * Set type to King
      */
    void becomeKing();
    /**
      * Returns true, if disc is a king.
      */
    bool isKing(){return m_type == King;}

    //Animation Section
    void jumpUp();
    void jumpDown();
    /**
     * @brief finishAnimation ensure final state
     */
    void finishAnimation() override;
    /**
     * @brief updateAnimatedProperties updates lift vector
     * @param animationState
     */
    void updateAnimatedProperties(float animationState)Q_DECL_OVERRIDE;
    /**
     * @brief calculateDrawMatrix add the animation to  m_transformationMatrix
     */
    void calculateDrawMatrix() Q_DECL_OVERRIDE;


private:
    GLfloat m_height;
    int m_slices;
    QPoint m_fieldCoord;
    DiscType m_type;

    //Animation
    QVector3D m_liftVector;
    QVector3D m_finalLiftVector;
    QVector3D m_startLiftVector;
}; // GLDISC_H
#endif
