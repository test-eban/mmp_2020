 #include "glboard.h"

GLBoard::GLBoard(GLColorRgba color, const QVector3D & center)
    :GLBody("Board", 1.0, color)
{
    m_center = center;

    fillBoard();
}

void GLBoard::printBoard()
{
// Displaying the 2D vector
//    for (int i = 0; i < board.size(); i++)
//    {
//        for (int j = 0; j < board[i].size(); j++)
//        {
//            std::cout << board[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }

    std::cout << "  0 1 2 3 4 5 6" << std::endl;
    for (int i = 0; i < board[0].size(); i++)
    {
        std::cout << i << " ";
        for (int j = 0; j < board.size(); j++)
            std::cout << board[j][i] << " ";
        std::cout << std::endl;
    }
}

int GLBoard::addToken(int x, int player)
{
    // an dieser Stelle passieren die folgenden Dinge:
    // Stein wird an Spalte X eingefügt und gibt dann die Position (y) zurück, in welcher sich der Stein befindet
    // Sollte der Stein an eine ordentliche Position eingefügt werden (x und y zwischen 0 und 7)
    // wird dann geprüft, ob eine winning condition gegeben ist. Der Aufruf erfolgt vom MyGlItem

//    int amount = board[x].size();
    int amount = 0;

    for (int i = 0; i < 7; i++)
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
        int nextFree = 6-amount;
        board[x][nextFree] = player;

        lastX = x;
        lastY = nextFree;

        return nextFree;
    }

}

bool GLBoard::checkForWin()
{
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X

    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 0 0 X X X X
    // X X X X 0 0 0 0 0 R Y X X X X
    // X X X X 0 0 0 0 R Y R X X X X
    // X X X X 0 R Y Y Y R R X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X
    // X X X X X X X X X X X X X X X

    //
    //
    //
    //
    //
    //
    //
    //

    // ol    or
    // ll    rr
    // ul uu ur

    bool win = false;
    // uu (vertikal)
//    if (lastY < 3)
//    {
//        win = board[lastX][lastY] == board[lastX][lastY-1] == board[lastX][lastY-2] == board[lastX][lastY-3];
//    }

    // ll rr (horizontal)
    // maximaler radius, in welchem etwas passieren kann ist +- 3, also von -3 bis 3
    unsigned int player = board[lastX][lastY];
    unsigned int hitCount = 0;

//    board[x-3][y] == board[x-2][y] == board[x-1][y] == board[x][y];
//    board[x-3][y] == board[x-2][y] == board[x-1][y] == board[x][y];
//    board[x-2][y] == board[x-1][y] == board[x-1][y] == board[x][y];

//    for (int i = 0; i < board[lastX].size(); i++)
//    {
//        if (board[lastX][i] == player)
//        {
//            hitCount += 1;
//        }
//        else if (hitCount < 4)
//        {
//            hitCount = 0;
//        }
//    }


    // horizontalCheck
       for (int j = 0; j<board[0].size()-3 ; j++ ){
           for (int i = 0; i<board.size(); i++){
               if (board[i][j] == player && board[i][j+1] == player && board[i][j+2] == player && board[i][j+3] == player){
                   return true;
               }
           }
       }
       // verticalCheck
       for (int i = 0; i<board.size()-3 ; i++ ){
           for (int j = 0; j<this->board[0].size(); j++){
               if (board[i][j] == player && board[i+1][j] == player && board[i+2][j] == player && board[i+3][j] == player){
                   return true;
               }
           }
       }
       // ascendingDiagonalCheck
       for (int i=3; i<board.size(); i++){
           for (int j=0; j<board[0].size()-3; j++){
               if (board[i][j] == player && board[i-1][j+1] == player && board[i-2][j+2] == player && board[i-3][j+3] == player)
                   return true;
           }
       }
       // descendingDiagonalCheck
       for (int i=3; i<board.size(); i++){
           for (int j=3; j<board[0].size(); j++){
               if (board[i][j] == player && board[i-1][j-1] == player && board[i-2][j-2] == player && board[i-3][j-3] == player)
                   return true;
           }
       }
       return false;
}

void GLBoard::fillBoard()
{
    board.resize(7, std::vector<int>(6, 0));
}
