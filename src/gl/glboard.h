#ifndef GLBOARD_H
#define GLBOARD_H

#include "glbody.h"
#include <iostream>

/**
 * @brief The classic connect four board
 */
class GLBoard : public GLBody
{
public:
    /**
     * @brief constructor - calls baseclassconstructor
     * @param color color of the board
     * @param center center of the board
     */
    GLBoard(GLColorRgba color, const QVector3D & center = v_Zero);

    /**
     * @brief inserts a token into the board
     * @param x column
     * @param player inserting player
     * @return row height
     */
    int insertToken(unsigned int x, unsigned int player);

    /**
     * @brief checks if a player has won
     * @return true if win, false if no win
     */
    bool checkForWin();

    /**
     * @brief returns the filepath of the binaryObjectFile
     * @return
     */
    QString getFile() { return m_binaryFilePath; }

    /**
     * @brief prints the board to console
     */
    void printBoard();

private:
    /**
     * @brief fills the board
     */
    void fillBoard();

    const QString m_binaryFilePath = "/home/testeban/Desktop/MM2020_01/models/brett.dat";
    std::vector<std::vector<unsigned int>> board;

    unsigned int lastX;
    unsigned int lastY;
};

#endif // GLBOARD_H
