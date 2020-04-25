#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glmultiplebody.h"

/**
 * @brief This class provides everything necessary to work with the tokens. It utilizes GLMultipleBody to archieve a better framerate.
 */
class GLToken : public GLMultipleBody
{
public:
    /**
     * @brief constructor. calls baseclassconstructor, sets the color and appends this object into the copies container.
     * @param color color of the token
     */
    GLToken(GLColorRgba color);

    // Animation Section
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

    /**
     * @brief returns the filepath of the binaryObjectFile
     * @return
     */
    QString getFile() { return m_binaryFilePath; }

    /**
     * @brief getter for m_movable
     * @return m_movable
     */
    bool isMovable() { return m_movable; }

    /**
     * @brief setter for m_movable
     * @param new val for m_movable
     */
    void setMovable(bool movable) { m_movable = movable; }

private:
    bool m_movable; // determines if the token can and should be moved by the connectfourclass

    // Animation
    QVector3D m_liftVector;
    QVector3D m_finalLiftVector;
    QVector3D m_startLiftVector;

    // File
    const QString m_binaryFilePath = ":/models/stein.dat";
};

#endif // GLTOKEN_H


