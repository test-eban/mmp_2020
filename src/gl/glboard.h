#ifndef GLBOARD_H
#define GLBOARD_H

#include "glbody.h"
#include <iostream>

class GLBoard : public GLBody
{
public:
    GLBoard(GLColorRgba color, const QVector3D & center = v_Zero);

    int addToken(int x, int player);

    bool checkForWin();

    QString getFile() { return m_binaryFilePath; }

    void printBoard();

private:
    void fillBoard();

//    const QString m_binaryFilePath = "/home/testeban/Desktop/wvfrontexport/final/brett.dat";
    const QString m_binaryFilePath = "/home/testeban/Desktop/MM2020_01/models/brett.dat";
    std::vector<std::vector<int>> board;

    unsigned int lastX;
    unsigned int lastY;
};

#endif // GLBOARD_H
