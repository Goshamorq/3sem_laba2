#pragma once

#include <QDebug>
#include "tree.h"
enum class gameState {FINISHED, TIE, CONTINUES};

class TicTacToeCore
{
public:
    explicit TicTacToeCore(std::array<char, 9> arr);
    void computerPlay();
    gameState gameStatus() const;
    void clearBoard();
    void changeGameDifficulty(int gameDifficulty);
    bool AI_first = false;
    std::array<char, 9> cells;
    Tree_of_variations<'X', 'O'> tree;
    friend QDebug operator<<(QDebug stream, const TicTacToeCore &TTT);

private:
    void computerPlaySmart();
    int m_gameDifficulty = 0;
};
