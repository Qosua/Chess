#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QGraphicsPixmapItem>

class ChessPiece : public QGraphicsPixmapItem{
    
    
public:
    ChessPiece(const QString& path, const int cellSize);
    ~ChessPiece();
    
};

#endif // CHESSPIECE_H
