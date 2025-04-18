#include "winwidget.h"
#include "ui_winwidget.h"

WinWidget::WinWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::WinWidget) {
    m_ui->setupUi(this);
    
    connect(m_ui->closeButton, &QPushButton::clicked, this, &WinWidget::closeWidget);
    
}

WinWidget::~WinWidget()
{
    delete m_ui;
}

void WinWidget::setFlags(bool isWin, bool color) {
    
    if(isWin){
        
        if(!color){
            m_ui->label->setText("White win");
            m_ui->icon->setIcon(QIcon(":/icons/wk.png"));
            
        }
        else{
            m_ui->label->setText("Black win");
            m_ui->icon->setIcon(QIcon(":/icons/bk.png"));
            
        }
        
    }
    else{
        
        m_ui->label->setText("Draw");
        m_ui->icon->setIcon(QIcon(""));
        
    }
    
}
