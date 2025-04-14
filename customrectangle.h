#ifndef CUSTOMRECTANGLE_H
#define CUSTOMRECTANGLE_H

#include <QGraphicsRectItem>
#include <QPainter>

class CustomRectangle : public QGraphicsRectItem{
public:
    CustomRectangle(const QRectF& rect);
    
    void setText(QString str, QColor color, bool pos);
    
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    
    bool reverseSomeCell;
    
private:
    
    QString m_text;
    QColor m_color;
    bool m_pos;
    
};


#endif // CUSTOMRECTANGLE_H
