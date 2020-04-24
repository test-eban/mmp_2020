#include "glmask.h"


GLMask::GLMask(const QString &name, float radius, const QVector3D &center, const GLColorRgba &color, const QString textureFile)
    :GLBody(name, radius, color,textureFile)
{
    m_startRadius = 0.0;
    m_endRadius = m_radius;
    setCenter(center);
}

void GLMask::draw(GLESRenderer *renderer)
{
    QVector3D clipCenter = renderer->modelToClip(m_center);
    QVector3D viewportCenter = renderer->clipToViewport(clipCenter);
    QPoint maskCenter (QPoint(static_cast<int>(viewportCenter.x()), static_cast<int>(viewportCenter.y())));
    renderer->setMaskCenter(maskCenter);
    renderer->setMaskDiameter(m_radius);
}

void GLMask::startAnimation(float startValue)
{
   m_radius = m_startRadius;
   m_animateForwards = true;
   m_animationStart = startValue;
   m_animationActive = true;
}

void GLMask::finishAnimation()
{
  m_radius = m_endRadius;
  m_animationActive = false;
}

void GLMask::updateAnimatedProperties(float animationState)
{
   m_radius = m_startRadius + (m_endRadius - m_startRadius) * animationState;
}
