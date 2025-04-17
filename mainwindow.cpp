#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    m_centralWidget = new QWidget;
    m_mainLayout = new QStackedLayout(m_centralWidget);
    m_menuWidget = new MenuWidget;
    m_classicalChess = new ClassicalChessWidget;
    m_chooseWidget = new ChoosePieceWidget;

    m_mainLayout->addWidget(m_menuWidget);
    m_mainLayout->addWidget(m_classicalChess);
    m_mainLayout->addWidget(m_chooseWidget);

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

        //TODO

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

}
