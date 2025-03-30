#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPen>

class ChessBoard : public QGraphicsScene{
    
public:
    ChessBoard();
    ~ChessBoard();
    
    void setupBoard();
    
private:
    
    void drawField();
    void drawPieces();
    
    size_t m_cellSize = 80;
    
    
};

#endif // CHESSBOARD_H
