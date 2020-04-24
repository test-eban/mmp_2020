#ifndef GLTOKENTRAY_H
#define GLTOKENTRAY_H

#include "glbody.h"

/*
 * /---------------------\
 * |CCCCCCCCCCCCCCCCCCCCC|
 * \---------------------/
 */
/**
 * @brief Holding tray for the tokens. A tray can fit 21 tokens.
 */
class GLTokenTray : public GLBody
{
public:
    /**
     * @brief constructor. calls baseclassconstructor
     * @param color color of the tray
     */
    GLTokenTray(GLColorRgba color);

    /**
     * @brief returns the filepath of the binaryObjectFile
     * @return
     */
    QString getFile() { return m_binaryFilePath; }

private:
    const QString m_binaryFilePath = "/home/testeban/Desktop/MM2020_01/models/spielsteinablagebank.dat";
};

#endif // GLTOKENTRAY_H
