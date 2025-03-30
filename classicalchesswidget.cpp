#include "classicalchesswidget.h"

ClassicalChessWidget::ClassicalChessWidget(QWidget* parent) :
	QWidget(parent), m_ui(new Ui::ClassicalChessWidget) {

	m_ui->setupUi(this);
    
    establishingConnections();
    
}

ClassicalChessWidget::~ClassicalChessWidget() {

	delete m_ui;

}

void ClassicalChessWidget::openMatchMaker() {
    
    m_ui->stackedWidget->setCurrentIndex(0);
    


}

void ClassicalChessWidget::openMatchFinder() {
    
    m_ui->stackedWidget->setCurrentIndex(2);
    
    //client setup;
    
    


}

void ClassicalChessWidget::establishingConnections() {
    
    connect(m_ui->close, &QPushButton::clicked, this, [this](){
        
        emit closeWidget();
        m_ui->infoLabel->clear();
        m_choosenSide = 0;
        m_ui->chooseWhite->setEnabled(true);
        m_ui->chooseBlack->setEnabled(true);
        m_ui->chooseWhite->setStyleSheet("background-color: #21201d");
        m_ui->chooseBlack->setStyleSheet("background-color: #21201d");
        m_waitingOpponent = false;
        
    });
    connect(m_ui->close_2, &QPushButton::clicked, this, [this](){
        
        emit closeWidget();
        m_ui->infoLabel_2->clear();
        m_ui->ipLine->clear();
        
    });
    connect(m_ui->chooseWhite, &QPushButton::clicked, this, [this](){
        
        m_ui->chooseWhite->setStyleSheet("background-color: #3e3e3e");
        m_ui->chooseBlack->setStyleSheet("background-color: #21201d");
        m_choosenSide = 1;
        
    });
    connect(m_ui->chooseBlack, &QPushButton::clicked, this, [this](){
        
        m_ui->chooseBlack->setStyleSheet("background-color: #3e3e3e");
        m_ui->chooseWhite->setStyleSheet("background-color: #21201d");
        m_choosenSide = 2;
        
    });
    connect(m_ui->startMatch, &QPushButton::clicked, this, &ClassicalChessWidget::startMatch);
    connect(m_ui->connectToHost, &QPushButton::clicked, this, &ClassicalChessWidget::connectToHost);
    
}

void ClassicalChessWidget::startMatch() {
    
    if(m_waitingOpponent)
        return;
    
    if(m_choosenSide == 0){
        
        m_ui->infoLabel->setText("You need to choose a side");
        m_ui->infoLabel->setStyleSheet("color: red");
        return;
        
    }
    
    m_ui->infoLabel->setText("Waiting for opponent");
    m_ui->infoLabel->setStyleSheet("color: white");
    m_ui->chooseWhite->setEnabled(false);
    m_ui->chooseBlack->setEnabled(false);
    m_waitingOpponent = true;
    
    //server setup;
    m_ui->stackedWidget->setCurrentIndex(1);
    
    m_board = new ChessBoard;
    m_board->setupBoard();
    
    m_ui->graphicsView->setScene(m_board);
    m_ui->graphicsView->scene()->setSceneRect(QRect(0,0, 640,640));
    m_ui->graphicsView->fitInView(m_ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
    
}
void ClassicalChessWidget::connectToHost() {
    
    if(m_ui->ipLine->text().isEmpty()){
        m_ui->infoLabel_2->setText("You need to enter IP address");
        m_ui->infoLabel_2->setStyleSheet("color: red");
    }
    
    QString address = m_ui->connectToHost->text();
    
}

void ClassicalChessWidget::resizeEvent(QResizeEvent *event) {
    
    if(m_ui->graphicsView->scene() != nullptr){
        m_ui->graphicsView->scene()->setSceneRect(QRect(0,0, 640,640));
        m_ui->graphicsView->fitInView(m_ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
    
}
























