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
    GLTokenTray(GLColorRgba color);

    QString getFile() { return m_binaryFilePath; }

private:
    const QString m_binaryFilePath = "/home/testeban/Desktop/MM2020_01/models/spielsteinablagebank.dat";
};

#endif // GLTOKENTRAY_H
