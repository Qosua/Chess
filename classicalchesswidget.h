#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_classicalchesswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ClassicalChessWidget;
}
QT_END_NAMESPACE


class ClassicalChessWidget : public QWidget {

	Q_OBJECT

public:

	ClassicalChessWidget(QWidget* parent = nullptr);
	~ClassicalChessWidget();
    
signals:
    
    void closeWidget();

public slots:

	void openMatchMaker();
	void openMatchFinder();

private:

	Ui::ClassicalChessWidget* m_ui = nullptr;
    int m_choosenSide = 0; // 0 - havent choosen, 1 - white, 2 - black;
    bool m_waitingOpponent = false;
    
    void establishingConnections();
    void startMatch();
    void connectToHost();

};


