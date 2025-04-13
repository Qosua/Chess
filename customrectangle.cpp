#include "customrectangle.h"

CustomRectangle::CustomRectangle(const QRectF& rect) : QGraphicsRectItem(rect) {}

void CustomRectangle::setText(QString str, QColor color, bool pos) {
    
    m_text = str;
    m_color = color;
    m_pos = pos;
    update();
    
}

void CustomRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    
    QGraphicsRectItem::paint(painter, option, widget);
    
    QFont font("Arial", 12);
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(m_color);
    
    QFontMetrics metrics(painter->font());
    QRect textRect = metrics.boundingRect(m_text);
    
    qreal x;
    qreal y;
    
    if(m_pos) {
        x = rect().right() - textRect.width() - 3;
        y = rect().bottom() - 5;
    }
    else{
        x = rect().left() + textRect.width() - 3;
        y = rect().top() + 16;
    }
    
    painter->drawText(QPointF(x,y), m_text);
    
    if(m_text == 'a' or m_text == 'A'){
        
            x = rect().left() + textRect.width() - 3;
            y = rect().top() + 16;
        
        painter->drawText(QPointF(x,y), "1");
        
    }
    
}
