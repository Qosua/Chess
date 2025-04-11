#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPen>
#include <vector>
#include <QGraphicsBlurEffect>
#include <cmath>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <windows.h>

#include "chesspiece.h"

class ChessBoard : public QGraphicsScene{
    
    
public:
    ChessBoard();
    ~ChessBoard();
    
    void setupBoard();
    void setPlayerSide(bool side);
    bool getPlayerSide();
    
private:
    void drawField();
    void drawPieces();
    void preparePieces();
    void validateTurn(QPointF newPos, QPointF oldPos);
    void validateTurnWithType(QPointF newPos, QPointF oldPos, ChessPiece* senderPiece);
    void changeCellColorAt(QPointF pos, bool isActivated);
    int abs(int num);
    void highlightTips(ChessPiece* senderPiece);
    void deleteTips();
    ChessPiece* findPeiceOnCoords(QPointF pos);
    void drawTipAt(qreal x, qreal y);
    bool isPieceOnWay(QPointF oldPos, QPointF newPos);
    void deletePieceAt(QPointF pos);
    void deletePiece(ChessPiece* pieceToDelete);
    bool isPieceChecked(ChessPiece* pieceToCheck);
    
    size_t m_cellSize = 80;
    std::vector<ChessPiece*> m_piecesArr;
    ChessPiece* m_lastChosenPiece = nullptr;
    QPointF m_lastChosenPos;
    bool m_playerSide = true;
    std::vector<QGraphicsEllipseItem*> m_tipsArr;
    ChessPiece* m_playerKing;
    ChessPiece* m_enemyKing;

    QMediaPlayer m_soundPlayer;
    QAudioOutput m_audioOutput;
    
    QString notifySoundPath = "C:/Repos/Qt/Chess/sounds/notify.mp3";
    QString moveSoundPath = "C:/Repos/Qt/Chess/sounds/move-self.mp3";
    QString checkSoundPath = "C:/Repos/Qt/Chess/sounds/move-check.mp3";
    QString captureSoundPath = "C:/Repos/Qt/Chess/sounds/capture.mp3";
    QString castleSoundPath = "C:/Repos/Qt/Chess/sounds/castle.mp3";

private slots:
    void catchChosenPiece(QPointF oldPos);
    
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // CHESSBOARD_H
