#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "unistd.h"
std::array<char,9> arr = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget), TTT(arr)
{
    ui->setupUi(this);
    this->setWindowTitle("Tic Tac Toe");
    pushButtons[0] = ui->button_1;
    pushButtons[1] = ui->button_2;
    pushButtons[2] = ui->button_3;
    pushButtons[3] = ui->button_4;
    pushButtons[4] = ui->button_5;
    pushButtons[5] = ui->button_6;
    pushButtons[6] = ui->button_7;
    pushButtons[7] = ui->button_8;
    pushButtons[8] = ui->button_9;

    updateGUI();
    for (int i = 0; i < 9; ++i)
    {
        connect(pushButtons[i], SIGNAL (clicked()), this, SLOT (userPlayed()));
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateGUI()
{
    for (int i = 0; i < 9; ++i)
    {

        QPushButton *button = pushButtons[i];
        button->setText(QString(TTT.cells[i]));

        switch (TTT.cells[i])
        {
        case 'X':
            button->setDisabled(true);
            if (TTT.AI_first)
            {
                button->setStyleSheet("background-color: rgb(252, 175, 62);color: blue;");
            }
            else
            {
                button->setStyleSheet("background-color: rgb(252, 175, 62);color: red;");
            }
            break;
        case 'O':
            button->setDisabled(true);
            if (TTT.AI_first)
            {
                button->setStyleSheet("background-color: rgb(252, 175, 62);color: red;");
            }
            else
            {
                button->setStyleSheet("background-color: rgb(252, 175, 62);color: blue;");
            }
            break;
        case ' ':
            button->setEnabled(true);
            button->setStyleSheet("background-color: rgb(252, 175, 62);");
            break;
        }
    }
    ui->labelScorePlayer->setStyleSheet("color: red;");
    ui->labelScoreTie->setStyleSheet("color: orange;");
    ui->labelScoreComputer->setStyleSheet("color: blue;");
    this->repaint();
}

void Widget::gameFinishedPlayerWon()
{
    m_score.player++;
    ui->labelScorePlayer->setNum(m_score.player);
    ui->labelScorePlayer->setStyleSheet("color: red;font-weight:bold;font-size:30px;");
    this->repaint();
    if (TTT.AI_first==false)
    {
        TTT.AI_first = ! TTT.AI_first;
    }
    usleep(1 * 1000000);//wait for it
    TTT.clearBoard();
    TTT.tree.restart();
    updateGUI();
}

void Widget::gameFinishedTieWon()
{
    m_score.tie++;
    ui->labelScoreTie->setNum(m_score.tie);
    ui->labelScoreTie->setStyleSheet("color: orange;font-weight:bold;font-size:30px;");

    TTT.AI_first = ! TTT.AI_first; // обновление хода

    this->repaint();
    usleep(1 * 1000000);//wait for it
    TTT.clearBoard();
    TTT.tree.restart();
    updateGUI();
}

void Widget::gameFinishedComputerWon()
{
    m_score.computer++;
    ui->labelScoreComputer->setNum(m_score.computer);
    ui->labelScoreComputer->setStyleSheet("color: blue;font-weight:bold;font-size:30px;");
    if (TTT.AI_first==true)
    {
        TTT.AI_first = ! TTT.AI_first;
    }
    this->repaint();
    usleep(1 * 1000000);//wait for it
    TTT.clearBoard();
    TTT.tree.restart();
    updateGUI();
}

void Widget::userPlayed()
{
    {
        int index = sender()->objectName().split("_")[1].toInt();
        TTT.cells[index-1] = TTT.AI_first? 'O':'X';
        TTT.tree.update_root(TTT.cells);
    }
    updateGUI();

    switch (TTT.gameStatus())
    {
    case gameState::FINISHED:
        gameFinishedPlayerWon();
        break;
    case gameState::TIE:
        gameFinishedTieWon();
        break;
    case gameState::CONTINUES:
        break;
    }

    TTT.computerPlay();
    updateGUI();
    switch (TTT.gameStatus())
    {
    case gameState::FINISHED:
        gameFinishedComputerWon();
        break;
    case gameState::TIE:
        gameFinishedTieWon();
        break;
    case gameState::CONTINUES:
        break;
    }
    updateGUI();
}

