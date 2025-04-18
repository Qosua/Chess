#ifndef WINWIDGET_H
#define WINWIDGET_H

#include <QWidget>

namespace Ui {
class WinWidget;
}

class WinWidget : public QWidget
{
    Q_OBJECT

public:
    WinWidget(QWidget *parent = nullptr);
    ~WinWidget();
    
    void setFlags(bool isWin, bool color);
    
signals:
    void closeWidget();
    
private:
    Ui::WinWidget *m_ui;
};

#endif // WINWIDGET_H
