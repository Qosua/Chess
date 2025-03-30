#include "chesspiece.h"

ChessPiece::ChessPiece(const QString& path, const int cellSize) {
    
    QPixmap pix(path);
    pix.scaled(cellSize,cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pix);
    this->setFlag(QGraphicsPixmapItem::ItemIsMovable);
    
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

void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    
    if(event->button() == Qt::MouseButton::LeftButton){
        
        m_previosPos = event->scenePos();
        m_previosPos.setX(int(m_previosPos.x()/80)*80);
        m_previosPos.setY(int(m_previosPos.y()/80)*80);
        
        this->setCursor(Qt::ClosedHandCursor);
        QGraphicsPixmapItem::mousePressEvent(event);
        
    }
    
}

void ChessPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    
    if(event->button() == Qt::MouseButton::LeftButton){
        
        QPointF m_newPos = event->scenePos();
        
        m_newPos.setX(int(m_newPos.x()/80)*80);
        m_newPos.setY(int(m_newPos.y()/80)*80);
        
        emit newPosition(m_newPos, m_previosPos);
        
        this->unsetCursor();
        QGraphicsPixmapItem::mouseReleaseEvent(event);
        
    }
    
}
    
