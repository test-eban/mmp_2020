#include "gltoken.h"

GLToken::GLToken(GLColorRgba color)
    : GLMultipleBody("Token")
{
    setColor(color);
    m_copies.append(this);

    m_movable = true;
}

void GLToken::jumpUp()
{
    m_finalLiftVector = v_Y*1.5;
    m_startLiftVector = v_Zero;
}

void GLToken::jumpDown()
{
    m_finalLiftVector = v_Zero;
    m_startLiftVector = m_liftVector;
}

void GLToken::finishAnimation()
{
    m_liftVector = m_finalLiftVector;
}

void GLToken::updateAnimatedProperties(float animationState)
{
   m_liftVector = m_startLiftVector + localAnimationState(animationState) *(m_finalLiftVector - m_startLiftVector);
}

void GLToken::calculateDrawMatrix()
{
  m_drawMatrix = m_transformationMatrix;
  m_drawMatrix.translate(m_liftVector);
}
