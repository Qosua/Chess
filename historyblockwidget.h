#ifndef HISTORYBLOCKWIDGET_H
#define HISTORYBLOCKWIDGET_H

#include <QWidget>

#include "chesspiece.h"
#include "ui_historyblockwidget.h"

enum ActionType {
    
    turn,
    capture,
    surrender,
    drawOffer,
    
};

struct PlayerAction {
    
    bool actionSide;
    
    PieceType pieceOne;
    QString pieceTwoName;
    
    QPointF oldPos;
    QPointF newPos;
    
    ActionType turnType;
    
    int turnNumber;
    
};

class HistoryBlockWidget : public QWidget {
    
    Q_OBJECT
    
public:
    HistoryBlockWidget(const PlayerAction& info);
    
private:
    
    Ui::HistoryBlockWidget* m_ui = nullptr;
    
};

#endif // HISTORYBLOCKWIDGET_H
