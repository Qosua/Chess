#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPen>
#include <vector>
#include <QGraphicsBlurEffect>
#include <cmath>

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
    void validateTurn(QPointF newPos, QPointF oldPos);
    void validateTurnWithType(QPointF newPos, QPointF oldPos, ChessPiece* senderPiece);
    void scaleScene();
    
    size_t m_cellSize = 80;
    std::vector<ChessPiece*> m_piecesArr;
    
};

#endif // CHESSBOARD_H
