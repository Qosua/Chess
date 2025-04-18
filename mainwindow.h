#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedLayout>

#include "menuwidget.h"
#include "classicalchesswidget.h"
#include "choosepiecewidget.h"
#include "winwidget.h"

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStackedLayout* m_mainLayout;
    MenuWidget* m_menuWidget;
    ClassicalChessWidget* m_classicalChess;
    QWidget* m_centralWidget;
    ChoosePieceWidget* m_chooseWidget;
    WinWidget* m_winWidget;

    void establishingConnections();

};
