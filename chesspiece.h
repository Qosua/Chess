#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsScene>
#include <QObject>

enum PieceType{
    
    blackPawn,
    whitePawn,
    bishop,
    knight,
    king,
    queen,
    rook,
    none,
    
};

class ChessPiece : public QObject, public QGraphicsPixmapItem {
    
    Q_OBJECT
    
public:
    ChessPiece(const QString& path, const int cellSize);
    ~ChessPiece();
    
    void setType(PieceType type);
    PieceType getType();
    void setPieceColor(bool color);
    bool getPieceColor();
    int getTurnsCount();
    void plusOneToTurn();
    void setMoveFlag(bool moveFlag);
    bool getMoveFlag();
    void setTexture(QString path);
    
signals:
    void newPosition(QPointF newPos, QPointF oldPos);
    void pieceIsChosen(QPointF oldPos);
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
private:
    PieceType m_type;
    QPointF m_previosPos;
    bool m_pieceColor;
    int m_cellSize;
    int m_turnsCount;
    bool m_moveFlag;
    
};

#endif // CHESSPIECE_H
