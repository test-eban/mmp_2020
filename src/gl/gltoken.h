#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"
#include "glmultiplebody.h"
#include "glcolorrgba.h"

/**
 * @brief The GLToken class TODO
 */
class GLToken : public GLMultipleBody
{
public:
    GLToken(GLColorRgba color);

    //simple setters
//    void setFieldCoord(const QPoint & newVal){m_fieldCoord = newVal;}

    //simple getters
//    const QPoint & getFieldCoord()const{return m_fieldCoord;}
//    GLfloat getHeight(){return m_height;}

//    /**
//      * Returns true, if discs are colliding
//      */
//    bool isColliding(const GLToken * other);

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
     * @brief calculateDrawMatrix add the animation to m_transformationMatrix
     */
    void calculateDrawMatrix() Q_DECL_OVERRIDE;

    QString getFile() {return m_binaryFilePath; }

    bool isMovable() { return m_movable; }

    void setMovable(bool movable) { m_movable = movable; }

private:
    int m_slices;
    bool m_movable;

    // Animation
    QVector3D m_liftVector;
    QVector3D m_finalLiftVector;
    QVector3D m_startLiftVector;

    // File
//    const QString m_binaryFilePath = "/home/testeban/Desktop/wvfrontexport/final/Stein.dat";
//    const QString m_binaryFilePath = "/home/testeban/Desktop/MM2020_01/models/lameStein.dat";
    const QString m_binaryFilePath = "/home/testeban/Desktop/MM2020_01/models/stein.dat";
};

#endif // GLTOKEN_H


