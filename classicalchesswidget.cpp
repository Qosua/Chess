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



}

void ClassicalChessWidget::establishingConnections() {
    
    connect(m_ui->close, &QPushButton::clicked, this, &ClassicalChessWidget::closeWidget);
    connect(m_ui->chooseWhite, &QPushButton::clicked, this, [this](){
        
        m_ui->chooseWhite->setStyleSheet("background-color: #3e3e3e");
        m_ui->chooseBlack->setStyleSheet("background-color: #21201d");
        
    });
    connect(m_ui->chooseBlack, &QPushButton::clicked, this, [this](){
        
        m_ui->chooseBlack->setStyleSheet("background-color: #3e3e3e");
        m_ui->chooseWhite->setStyleSheet("background-color: #21201d");
        
    });
    
}
