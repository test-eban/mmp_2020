#include "gltokentray.h"

GLTokenTray::GLTokenTray(GLColorRgba color, unsigned int amount)
    :GLBody("TokenTray", 1.0, color)
{
    m_amount = amount;
}

void GLTokenTray::removeToken()
{
    m_amount -= 1;
}
