#include "reg_window.h"
#include "main_window.h"
#include "auth_window.h"

RegWindow::RegWindow(QWidget *parent)
	: QMainWindow(parent), m_parent(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size()); // ������������ ������������� ������ ����
	// ��������� ������� ������������� ����� � ������� ������ � �������������
	connect(ui.nameEdit, &QLineEdit::textEdited, this, &RegWindow::nameEdit);
	connect(ui.passEdit, &QLineEdit::textEdited, this, &RegWindow::passEdit);
	connect(ui.passConfEdit, &QLineEdit::textEdited, this, &RegWindow::passConfEdit);
	connect(ui.regButton, &QPushButton::clicked, this, &RegWindow::regButton);
	connect(ui.authButton, &QPushButton::clicked, this, &RegWindow::authButton);
}

RegWindow::~RegWindow()
{}

// ��� ������� �� ������ ����������� ���������� ����� �����������, � ����� ����������� ��������
void RegWindow::authButton()
{
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // ��������� ������������ �������������� ��������� ������������� ������ ��� ������������� ������� MainWindow
	AuthWindow* auth = main->getAuthWindow();
	auth->show();
	this->hide();
}

// ��� ������� �� ������ �����������, ��������� ��������������� ����� ������ MainWindow
void RegWindow::regButton()
{
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // ��������� ������������ �������������� ��������� ������������� ������ ��� ������������� ������� MainWindow
	main->registrationUser();
}

// ��� �������� ����� ����������� ���������� ����� �����������
void RegWindow::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // ��������� ������������ �������������� ��������� ������������� ������ ��� ������������� ������� MainWindow
	AuthWindow* auth = main->getAuthWindow();
	auth->show();
}