#include "chesspiece.h"

ChessPiece::ChessPiece(const QString& path, const int cellSize) {
    
    QPixmap pix(path);
    pix.scaled(cellSize,cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pix);
    this->setFlag(QGraphicsPixmapItem::ItemIsMovable);
    
}

ChessPiece::~ChessPiece() {
    
    
    
}
    
