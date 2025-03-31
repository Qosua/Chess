#include "chesspiece.h"

ChessPiece::ChessPiece(const QString& path, const int cellSize) {
    
    QPixmap pix(path);
    pix.scaled(cellSize,cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pix);
    this->setFlag(QGraphicsPixmapItem::ItemIsMovable);
    m_cellSize = cellSize;
    m_turnsCount = 0;
    
}

ChessPiece::~ChessPiece() {
    
    
    
}

void ChessPiece::setType(PieceType type) {
    m_type = type;
}

PieceType ChessPiece::getType() {
    return m_type;
}

void ChessPiece::setPieceColor(bool color) {
    m_pieceColor = color;
}

bool ChessPiece::getPieceColor(){
    return m_pieceColor;
}

int ChessPiece::getTurnsCount() {
    return m_turnsCount;
}

void ChessPiece::plusOneToTurn() {
    m_turnsCount += 1;
}

void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    
    if(event->button() == Qt::MouseButton::LeftButton){
        
        m_previosPos = event->scenePos();
        m_previosPos.setX(int(m_previosPos.x()/m_cellSize)*m_cellSize);
        m_previosPos.setY(int(m_previosPos.y()/m_cellSize)*m_cellSize);

        emit pieceIsChosen(m_previosPos);
        
        this->setCursor(Qt::ClosedHandCursor);
        QGraphicsPixmapItem::mousePressEvent(event);
        
    }

}

void ChessPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    
    if(event->button() == Qt::MouseButton::LeftButton){
        
        QPointF m_newPos = event->scenePos();
        
        m_newPos.setX(int(m_newPos.x()/m_cellSize)*m_cellSize);
        m_newPos.setY(int(m_newPos.y()/m_cellSize)*m_cellSize);

        emit newPosition(m_newPos, m_previosPos);
        
        this->unsetCursor();
        QGraphicsPixmapItem::mouseReleaseEvent(event);
        
    }
    
}
    
