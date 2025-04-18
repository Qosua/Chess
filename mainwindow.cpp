#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    m_centralWidget = new QWidget;
    m_mainLayout = new QStackedLayout(m_centralWidget);
    m_menuWidget = new MenuWidget;
    m_classicalChess = new ClassicalChessWidget;
    m_chooseWidget = new ChoosePieceWidget;
    m_winWidget = new WinWidget;

    m_mainLayout->addWidget(m_menuWidget);
    m_mainLayout->addWidget(m_classicalChess);
    m_mainLayout->addWidget(m_chooseWidget);
    m_mainLayout->addWidget(m_winWidget);

    m_mainLayout->setCurrentIndex(0);
    m_mainLayout->setStackingMode(QStackedLayout::StackAll);

    this->establishingConnections();

    this->setCentralWidget(m_centralWidget);
    this->resize(1280, 720);
    
}

MainWindow::~MainWindow() {

    delete m_mainLayout;
    delete m_menuWidget;
    delete m_classicalChess;
    delete m_centralWidget;

}

void MainWindow::establishingConnections() {

    connect(m_menuWidget, &MenuWidget::moveToMakerClassical, this, [this]() {

        m_classicalChess->openMatchMaker();
        m_mainLayout->setCurrentIndex(1);

    });

    connect(m_menuWidget, &MenuWidget::moveToFinderClassical, this, [this]() {

        m_classicalChess->openMatchFinder();
        m_mainLayout->setCurrentIndex(1);

    });

    connect(m_menuWidget, &MenuWidget::moveToOfflineGame, this, [this]() {
        
        m_classicalChess->startOfflineMatch();
        m_mainLayout->setCurrentIndex(1);

    });
    
    connect(m_classicalChess, &ClassicalChessWidget::closeWidget, this, [this]() {
        
        m_mainLayout->setCurrentIndex(0);
        
    });
    
    connect(m_classicalChess, &ClassicalChessWidget::openPieceChoosingWidget, this, [this](ChessPiece* sender) {
        
        m_chooseWidget->setPieceToChange(sender);
        m_chooseWidget->setPiecesTextureByColor(sender->getPieceColor());
        m_mainLayout->setCurrentIndex(2);
        
    });
    
    connect(m_chooseWidget, &ChoosePieceWidget::closeWidget, this, [this](bool color) {
        
        m_mainLayout->setCurrentIndex(1);
        emit m_classicalChess->pieceChoosed(color);
        
        
    });
    connect(m_classicalChess, &ClassicalChessWidget::openWinInfoWidget, this, [this](bool isWin, bool color) {
        
        m_winWidget->setFlags(isWin, color);
        m_mainLayout->setCurrentIndex(3);
        
    });
    connect(m_winWidget, &WinWidget::closeWidget, this, [this]() {
        
        emit m_classicalChess->clearBoard();
        m_classicalChess->clearInfo();
        m_mainLayout->setCurrentIndex(0);
        
    });

}
