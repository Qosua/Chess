#include "menuwidget.h"

MenuWidget::MenuWidget(QWidget* parent) :
	QWidget(parent), m_ui(new Ui::MenuWidget) {

	m_ui->setupUi(this);

	connect(m_ui->MakeGameCC, &QPushButton::clicked, this, &MenuWidget::moveToMakerClassical);
	connect(m_ui->FindGameCC, &QPushButton::clicked, this, &MenuWidget::moveToFinderClassical);
	connect(m_ui->offlineGame, &QPushButton::clicked, this, &MenuWidget::moveToOfflineGame);
}

MenuWidget::~MenuWidget() {

	delete m_ui;

}
