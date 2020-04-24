#ifndef GLTOKENTRAY_H
#define GLTOKENTRAY_H

#include "glbody.h"

/*
 * /---------------------\
 * |CCCCCCCCCCCCCCCCCCCCC|
 * \---------------------/
 */

class GLTokenTray : public GLBody
{
public:
    GLTokenTray(GLColorRgba color, unsigned int amount = 21);

    void removeToken();

    unsigned int getFrontIndex() { return m_amount; }

    QString getFile() { return m_binaryFilePath; }

private:
    const QString m_binaryFilePath = "/home/testeban/Desktop/MM2020_01/models/spielsteinablagebank.dat";
    unsigned int m_amount; // beschreibt die Stelle, an welcher sich der vorderste Stein befindet.
};

#endif // GLTOKENTRAY_H
