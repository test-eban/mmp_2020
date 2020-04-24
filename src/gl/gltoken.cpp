#include "gltoken.h"

#include "math.h"

GLToken::GLToken(GLColorRgba color)
    : GLMultipleBody("Token")
{
    setColor(color);
    m_copies.append(this);
    m_movable = true;
}

void GLToken::calculateDrawMatrix()
{
  m_drawMatrix = m_transformationMatrix;
  m_drawMatrix.translate(m_liftVector);
}

///**
//  * Returns true, if GLDiscs are colliding
//  */
//bool GLToken::isColliding(const GLToken * other)
//{
//    if(this == other)
//        return false;

//    if((m_center - other->m_center).length() > m_radius + other->m_radius)
//        return false;
//    else
//    {
//        if(fabs(m_center.y() - other->m_center.y()) >((m_height + other->m_height) / 2.0f))
//            return false;
//        else return true;
//    }
//}

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
