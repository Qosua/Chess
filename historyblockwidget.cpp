#include "historyblockwidget.h"

HistoryBlockWidget::HistoryBlockWidget(const PlayerAction& info) :
    m_ui(new Ui::HistoryBlockWidget) {
    m_ui->setupUi(this);
    
    
    QString name;
    name = (info.actionSide ? "w" : "b");
    
    if(info.pieceOne == PieceType::bishop)
        name += "b";
    if(info.pieceOne == PieceType::queen)
        name += "q";
    if(info.pieceOne == PieceType::king)
        name += "k";
    if(info.pieceOne == PieceType::knight)
        name += "h";
    if(info.pieceOne == PieceType::rook)
        name += "r";
    if(info.pieceOne == PieceType::blackPawn or info.pieceOne == PieceType::whitePawn)
        name += "p";
    
    m_ui->pieceName->setText(name);
    
    int j = info.oldPos.x();
    int i = info.oldPos.y();
    QString from;
    from += QChar('a' + j/80);
    from += QChar('8' - i/80);
    m_ui->from->setText(from);
    
    j = info.newPos.x();
    i = info.newPos.y();
    QString to;
    to += QChar('a' + j/80);
    to += QChar('8' - i/80);
    m_ui->to->setText(to);
    
    m_ui->count->setText(QString::number(info.turnNumber));
    
    if(info.actionSide){
        m_ui->widget_3->setStyleSheet("background-color: #dedede");
        m_ui->time->setStyleSheet("color: #1e1e1e");
    }
    else{
        m_ui->widget_3->setStyleSheet("background-color: #1e1e1e");
        m_ui->time->setStyleSheet("color: #dedede");
    }
    
    if(!info.pieceTwoName.isEmpty()){
        
        m_ui->takedPiece->setText(info.pieceTwoName);
        m_ui->actionIcon->setText(" X");
        
    }

    QString minutes;
    QString seconds;
    QString hour;
    QString time;

    m_next = QDateTime::currentDateTime().toSecsSinceEpoch();

    qint64 deltaTime = m_next-m_prev;

    minutes = QString::number(deltaTime/60);
    seconds = QString::number(deltaTime%60);
    hour = QString::number(deltaTime/3600);

    if(seconds.size() == 1)
        seconds = '0' + seconds;

    if(minutes.size() == 1)
        minutes = '0' + minutes;

    if(hour.size() != 0)
        time = hour + ':';

    time += minutes + ":" + seconds;

    m_ui->time->setText(time);

    m_prev = m_next;
    

}

void HistoryBlockWidget::resetTime() {

    m_prev = QDateTime::currentDateTime().toSecsSinceEpoch();


}
