#include "chesspiece.h"

ChessPiece::ChessPiece(const QString& path, const int cellSize) {
    
    QPixmap pix(path);
    pix.scaled(cellSize,cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pix);
    m_cellSize = cellSize;
    m_turnsCount = 0;

    if(m_pieceColor == m_movebleColor)
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

int ChessPiece::getTurnsCount() {
    return m_turnsCount;
}

void ChessPiece::plusOneToTurn() {
    m_turnsCount += 1;
}

void ChessPiece::setMovebleColor(bool movebleColor) {

    m_movebleColor = movebleColor;

}

bool ChessPiece::getMovebleColor() {

    return m_movebleColor;

}

void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if(m_movebleColor != m_pieceColor)
        //return;
    
    qDebug() << "Piece has been pressed";
    if(event->button() == Qt::MouseButton::LeftButton){
        
        m_previosPos = event->scenePos();
        m_previosPos.setX(int(m_previosPos.x()/m_cellSize)*m_cellSize);
        m_previosPos.setY(int(m_previosPos.y()/m_cellSize)*m_cellSize);

        emit pieceIsChosen(m_previosPos);
        
        //only for visual
        QPointF cellCenter = m_previosPos + QPointF(m_cellSize/2, m_cellSize/2);
        this->setPos(this->pos() - (cellCenter - event->scenePos()));
        this->setCursor(Qt::ClosedHandCursor);
        
        QGraphicsPixmapItem::mousePressEvent(event);
        
    }

}
