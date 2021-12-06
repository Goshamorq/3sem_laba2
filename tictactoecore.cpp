#include "tictactoecore.h"
#include <QDebug>
#include "unistd.h"
#include "tree.h"

using namespace std;

TicTacToeCore::TicTacToeCore(std::array<char, 9> arr) : tree(arr), cells(arr)
{}

void TicTacToeCore::computerPlay()
{
    computerPlaySmart();
}

gameState TicTacToeCore::gameStatus() const
{

    if (tree.cur_root->check_win() == -1 || tree.cur_root->check_win() == 1) //check rows
    {
        return gameState::FINISHED;
    }
    if (find_ind(cells) !=-1)
    {
        return gameState::CONTINUES;
    }
    return gameState::TIE;
}

void TicTacToeCore::clearBoard()
{
    for (int i = 0; i < N; i++)
    {
        cells[i] = ' ';
    }
}

void TicTacToeCore::changeGameDifficulty(int gameDifficulty)
{
    m_gameDifficulty = gameDifficulty;
}


void TicTacToeCore::computerPlaySmart()
{
    usleep(0.2 * 1000000);//wait for it
    srand(time(nullptr));
    cells[tree.find_optimal(AI_first)] = AI_first?'X':'O';
    tree.update_root(cells);

}

QDebug operator<<(QDebug stream, const TicTacToeCore &TTT)
{
    QDebugStateSaver saver(stream);
    for (int i = 0; i < N; i++)
    {
        if ((i + 1) % NROOT == 0)
            stream.nospace() << TTT.cells[i] << "\n";
        else
            stream.nospace() << TTT.cells[i] << " | ";
    }
    return stream;
}

