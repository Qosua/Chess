#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPen>
#include <vector>
#include <set>
#include <QGraphicsBlurEffect>
#include <cmath>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "chesspiece.h"
#include "historyblockwidget.h"
#include "customrectangle.h"

class ChessBoard : public QGraphicsScene{
    
    Q_OBJECT
    
public:
    ChessBoard();
    ~ChessBoard();
    
    void setupBoard();
    void setPlayerSide(bool side);
    bool getPlayerSide();
    
signals:
    void turnMade(PlayerAction& actionInfo);
    
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
    bool deletePiece(ChessPiece* pieceToDelete);
    bool isPieceChecked(ChessPiece* pieceToCheck);
    QString getPieceName(ChessPiece* piece);
    void setAttackersPiecesFor(ChessPiece *pieceToCheck);
    bool isKingStalemated(ChessPiece *pieceToCheck);
    bool isKingMated(ChessPiece *pieceToCheck);
    bool isKingCanGoOutofCheck(ChessPiece* pieceToCheck);
    bool isPieceAbleToMoveAt(QPointF coords, ChessPiece* senderPiece);
    void clearBoard();
    void checkMateFor(bool color);
    void staleMate();
    
    size_t m_cellSize = 80;
    std::vector<ChessPiece*> m_piecesArr;
    ChessPiece* m_lastChosenPiece = nullptr;
    QPointF m_lastChosenPos;
    bool m_playerSide = true;
    std::vector<QGraphicsEllipseItem*> m_tipsArr;
    std::set<ChessPiece*> attackerPieces;
    ChessPiece* m_playerKing;
    ChessPiece* m_enemyKing;
    ChessPiece* tempIgnoredPiece = nullptr;
    size_t turnsCounter = 0;

    QMediaPlayer m_notifySound;
    QMediaPlayer m_moveSound;
    QMediaPlayer m_checkSound;
    QMediaPlayer m_captureSound;
    QMediaPlayer m_castleSound;
    
    QAudioOutput m_audioOutput1;
    QAudioOutput m_audioOutput2;
    QAudioOutput m_audioOutput3;
    QAudioOutput m_audioOutput4;
    QAudioOutput m_audioOutput5;
    
    QString notifySoundPath = "C:/Repos/Qt/Chess/sounds/notify.wav";
    QString moveSoundPath = "C:/Repos/Qt/Chess/sounds/move-self.wav";
    QString checkSoundPath = "C:/Repos/Qt/Chess/sounds/move-check.wav";
    QString captureSoundPath = "C:/Repos/Qt/Chess/sounds/capture.wav";
    QString castleSoundPath = "C:/Repos/Qt/Chess/sounds/castle.wav";

private slots:
    void catchChosenPiece(QPointF oldPos);
    
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // CHESSBOARD_H
