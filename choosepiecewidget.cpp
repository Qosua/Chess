#include "choosepiecewidget.h"
#include "ui_choosepiecewidget.h"

ChoosePieceWidget::ChoosePieceWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ChoosePieceWidget) {
    m_ui->setupUi(this);
    
    connect(m_ui->horse, &QPushButton::clicked, this, [this](bool clicked){
        
        m_choosedType = PieceType::knight;
        m_ui->horse->setStyleSheet("background-color: #3e3e3e;");
        m_ui->bishop->setStyleSheet("background-color: transparent;");
        m_ui->rook->setStyleSheet("background-color: transparent;");
        m_ui->queen->setStyleSheet("background-color: transparent;");
        
    });
    
    connect(m_ui->bishop, &QPushButton::clicked, this, [this](bool clicked){
        
        m_choosedType = PieceType::bishop;
        m_ui->horse->setStyleSheet("background-color: transparent;");
        m_ui->bishop->setStyleSheet("background-color: #3e3e3e;");
        m_ui->rook->setStyleSheet("background-color: transparent;");
        m_ui->queen->setStyleSheet("background-color: transparent;");
        
    });
    
    connect(m_ui->rook, &QPushButton::clicked, this, [this](bool clicked){
        
        m_choosedType = PieceType::rook;
        m_ui->horse->setStyleSheet("background-color: transparent;");
        m_ui->bishop->setStyleSheet("background-color: transparent;");
        m_ui->rook->setStyleSheet("background-color: #3e3e3e;");
        m_ui->queen->setStyleSheet("background-color: transparent;");
        
    });
    
    connect(m_ui->queen, &QPushButton::clicked, this, [this](bool clicked){
        
        m_choosedType = PieceType::queen;
        m_ui->horse->setStyleSheet("background-color: transparent;");
        m_ui->bishop->setStyleSheet("background-color: transparent;");
        m_ui->rook->setStyleSheet("background-color: transparent;");
        m_ui->queen->setStyleSheet("background-color: #3e3e3e;");
        
    });
    
    connect(m_ui->confirmButton, &QPushButton::clicked, this, [this](bool clicked){
        
        qDebug() << (m_choosedPiece == nullptr ? "ERROR" : "");
        
        m_choosedPiece->setType(m_choosedType);
        switch (m_choosedType) {
        case PieceType::knight:
            
            if(m_pieceColor)
                m_choosedPiece->setTexture(":/icons/wn.png");
            else
                m_choosedPiece->setTexture(":/icons/bn.png");
                
            break;
        case PieceType::bishop:
            
            if(m_pieceColor)
                m_choosedPiece->setTexture(":/icons/wb.png");
            else
                m_choosedPiece->setTexture(":/icons/bb.png");
            
            
            break;
        case PieceType::rook:
            
            if(m_pieceColor)
                m_choosedPiece->setTexture(":/icons/wr.png");
            else
                m_choosedPiece->setTexture(":/icons/br.png");
            
            
            break;
        case PieceType::queen:
            
            if(m_pieceColor)
                m_choosedPiece->setTexture(":/icons/wq.png");
            else
                m_choosedPiece->setTexture(":/icons/bq.png");
            
            
            break;
        default:
            break;
        }
        
        emit closeWidget(m_pieceColor);
        
    });
    
}

ChoosePieceWidget::~ChoosePieceWidget() {
    delete m_ui;
}

void ChoosePieceWidget::setPiecesTextureByColor(bool color) {
    
    m_pieceColor = color;
    
    if(color){
        
        m_ui->horse->setIcon(QIcon(":/icons/wn.png"));
        m_ui->bishop->setIcon(QIcon(":/icons/wb.png"));
        m_ui->rook->setIcon(QIcon(":/icons/wr.png"));
        m_ui->queen->setIcon(QIcon(":/icons/wq.png"));
        
    }
    else{
        
        m_ui->horse->setIcon(QIcon(":/icons/bn.png"));
        m_ui->bishop->setIcon(QIcon(":/icons/bb.png"));
        m_ui->rook->setIcon(QIcon(":/icons/br.png"));
        m_ui->queen->setIcon(QIcon(":/icons/bq.png"));
        
    }
    
}

void ChoosePieceWidget::setPieceToChange(ChessPiece *piece) {
    
    m_choosedPiece = piece;
    
}
