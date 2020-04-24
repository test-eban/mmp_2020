#include "glboard.h"

GLBoard::GLBoard(GLColorRgba color, const QVector3D & center)
    :GLBody("Board", 1.0, color)
{
    m_center = center;
    fillBoard();
}

void GLBoard::printBoard()
{
    std::cout << "  0 1 2 3 4 5 6" << std::endl;
    for (unsigned int i = 0; i < board[0].size(); i++)
    {
        std::cout << i << " ";
        for (unsigned int j = 0; j < board.size(); j++)
        {
            std::cout << board[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

int GLBoard::insertToken(unsigned int x, unsigned int player)
{
    int amount = 0;

    for (unsigned int i = 0; i < 7; i++)
    {
        if (board[x][i] != 0)
        {
            amount += 1;
        }
    }

    if (amount >= 7)
    {
        return -1;
    }
    else
    {
        int nextFree = 6 - amount;
        board[x][nextFree] = player;

        lastX = x;
        lastY = nextFree;

        return nextFree;
    }

}

bool GLBoard::checkForWin()
{
    unsigned int player = board[lastX][lastY];

    // horizontal
    for (unsigned int j = 0; j<board[0].size()-3 ; j++ )
    {
        for (unsigned int i = 0; i<board.size(); i++)
        {
            if (board[i][j] == player && board[i][j+1] == player && board[i][j+2] == player && board[i][j+3] == player)
            {
                return true;
            }
        }
    }

    // vertical
    for (unsigned int i = 0; i<board.size()-3 ; i++ )
    {
        for (unsigned int j = 0; j<this->board[0].size(); j++)
        {
            if (board[i][j] == player && board[i+1][j] == player && board[i+2][j] == player && board[i+3][j] == player)
            {
                return true;
            }
        }
    }

    // ascending diagonal
    for (unsigned int i=3; i<board.size(); i++)
    {
        for (unsigned int j=0; j<board[0].size()-3; j++)
        {
            if (board[i][j] == player && board[i-1][j+1] == player && board[i-2][j+2] == player && board[i-3][j+3] == player)
            {
                return true;
            }
        }
    }

    // descending diagonal
    for (unsigned int i=3; i<board.size(); i++)
    {
        for (unsigned int j=3; j<board[0].size(); j++)
        {
            if (board[i][j] == player && board[i-1][j-1] == player && board[i-2][j-2] == player && board[i-3][j-3] == player)
            {
                return true;
            }
        }
    }

    return false;
}

void GLBoard::fillBoard()
{
    board.resize(7, std::vector<unsigned int>(6, 0));
}
