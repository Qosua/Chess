#pragma once

#include <QWidget>
#include <QListWidgetItem>
#include "ui_classicalchesswidget.h"
#include "chessboard.h"

class ClassicalChessWidget : public QWidget {

	Q_OBJECT

public:
	ClassicalChessWidget(QWidget* parent = nullptr);
	~ClassicalChessWidget();
    
signals:
    void closeWidget();
    void openPieceChoosingWidget(ChessPiece* piece);
    void pieceChoosed(bool color);
    void openWinInfoWidget(bool winOrDraw, bool winForColor);
    void clearBoard();

public slots:
	void openMatchMaker();
	void openMatchFinder();
    void startOfflineMatch();
    void clearInfo();

private:
    ChessBoard* m_board = nullptr;
	Ui::ClassicalChessWidget* m_ui = nullptr;
    int m_choosenSide = 0; // 0 - havent choosen, 1 - white, 2 - black;
    bool m_waitingOpponent = false;
    
    void establishingConnections();
    void startMatch();
    void connectToMatch();
    void writeToHistory(PlayerAction& actionInfo);
    
protected:
    void resizeEvent(QResizeEvent *event);
};


