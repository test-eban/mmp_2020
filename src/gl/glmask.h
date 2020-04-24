#ifndef GLMASK_H
#define GLMASK_H

#include "glbody.h"
/**
 * @brief The GLMask class A mask to be rendered on top of a scene.
 *
 */
class GLMask : public GLBody
{
public:
    /**
     * @brief GLMask
     * @param name
     * @param radius Final radius in pixels.
     * @param center Center of mask in model coordinates
     * @param color Background color
     * @param textureFile Background texture
     */
    GLMask(const QString & name, float radius = 1.0,const  QVector3D& center = v_Zero, const GLColorRgba & color = GLColorRgba::clBlue,
           const QString textureFile = "");
    /**
     * @brief ~GLMask virtual destructor
     */
    virtual ~GLMask(){}

    /** Draws the surface and calls makeSurface if required.
     * Needs an active (made current) GL-Context.
     */
     virtual void draw(GLESRenderer * renderer);

    /**
     * @brief startAnimation Start an animation process.
     *        Sets animationActive to true.
     * Overwrite in subclasses to set Begin and End values for animated properties.
     */
    virtual void startAnimation(float startValue = 0.0);
    /**
     * @brief finishAnimation Finish an animation process.
     *        Sets animationActive to false.
     * Overwrite in subclasses to copy End values for animated properties to Begin values.
     */
    virtual void finishAnimation();

    /**
     * @brief updateAnimatedProperties
     * Overwrite to set current values of animated properties.
     */
    virtual void updateAnimatedProperties(float animationState);
private:
    float m_startRadius;
    float m_endRadius;
};

#endif // GLMASK_H
