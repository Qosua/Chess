#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPen>
#include <QVector>
#include <QGraphicsBlurEffect>

#include "chesspiece.h"

class ChessBoard : public QGraphicsScene{
    
public:
    ChessBoard();
    ~ChessBoard();
    
    void setupBoard();
    
private:
    
    void drawField();
    void drawPieces();
    void preparePieces();
    
    size_t m_cellSize = 80;
    QVector<ChessPiece*> m_piecesArr;
    
};

#endif // CHESSBOARD_H
