#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_menuwidget.h"

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget {

	Q_OBJECT

public:

	MenuWidget(QWidget* parent = nullptr);
	~MenuWidget();

signals:

	void moveToMakerClassical();
	void moveToFinderClassical();
	void moveToMakerNew();
	void moveToFinderNew();

private:

	Ui::MenuWidget* m_ui;

};

