#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_menuwidget.h"

class MenuWidget : public QWidget {

	Q_OBJECT

public:
	MenuWidget(QWidget* parent = nullptr);
	~MenuWidget();

signals:
	void moveToMakerClassical();
	void moveToFinderClassical();
    void moveToOfflineGame();

private:
	Ui::MenuWidget* m_ui;

};

