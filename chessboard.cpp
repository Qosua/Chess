#include "chessboard.h"

ChessBoard::ChessBoard() {
    
    
    
}
ChessBoard::~ChessBoard() {
    
    
    
}

void ChessBoard::setupBoard(){
    
    drawField();
    drawPieces();
    this->setBackgroundBrush(QBrush(QColor(38,37,34)));
    
}

void ChessBoard::drawField() {
    
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            
            QGraphicsRectItem* rectangle = new QGraphicsRectItem(i*m_cellSize, j*m_cellSize, m_cellSize, m_cellSize);
            
            if((i+j)%2 == 0)
                rectangle->setBrush(QBrush(QColor(235,236,208)));
            else
                rectangle->setBrush(QBrush(QColor(115,149,82)));
            
            rectangle->setPen(QPen(Qt::transparent, 0));
            
            this->addItem(rectangle);
            
        }
    }
    
}

void ChessBoard::drawPieces() {
    
    
    
}
