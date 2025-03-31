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
    ChessPiece* m_lastChosenPiece = nullptr;
    QPointF m_lastChosenPos;

private slots:
    void catchChosenPiece(QPointF oldPos);
    
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // CHESSBOARD_H
