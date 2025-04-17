#ifndef CHOOSEPIECEWIDGET_H
#define CHOOSEPIECEWIDGET_H

#include <QWidget>
#include "chesspiece.h"

namespace Ui {
class ChoosePieceWidget;
}

class ChoosePieceWidget : public QWidget
{
    Q_OBJECT

public:
    ChoosePieceWidget(QWidget *parent = nullptr);
    ~ChoosePieceWidget();
    
    void setPiecesTextureByColor(bool color);
    void setPieceToChange(ChessPiece* piece);
    
signals:
    void closeWidget(bool color);
    

private:
    Ui::ChoosePieceWidget *m_ui;
    PieceType m_choosedType = PieceType::queen;
    bool m_pieceColor;
    ChessPiece* m_choosedPiece;
};

#endif // CHOOSEPIECEWIDGET_H
