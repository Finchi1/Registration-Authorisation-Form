#include "auth_window.h"
#include "reg_window.h"
#include "main_window.h"

AuthWindow::AuthWindow(QWidget *parent)
	: QMainWindow(parent), m_parent(parent), m_successful_auth(false)
{
	ui.setupUi(this);
	this->setFixedSize(this->size()); // ������������ ������������� ������ ����
	// ��������� ������� ������������� ����� � ������� ������ � �������������
	connect(ui.nameEdit, &QLineEdit::textEdited, this, &AuthWindow::nameEdit);
	connect(ui.passEdit, &QLineEdit::textEdited, this, &AuthWindow::passEdit);
	connect(ui.authButton, &QPushButton::clicked, this, &AuthWindow::authButton);
	connect(ui.regButton, &QPushButton::clicked, this, &AuthWindow::regButton);
}

AuthWindow::~AuthWindow()
{}

// ��� ������� �� ������ �����������, ��������� ��������������� ����� ������ MainWindow
void AuthWindow::authButton()
{
	m_successful_auth = false;
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // ��������� ������������ �������������� ��������� ������������� ������ ��� ������������� ������� MainWindow
	main->authorisationUser();
}

// ��� ������� �� ������ ����������� ���������� ����� �����������, � ����� ����������� ��������
void AuthWindow::regButton()
{
	m_successful_auth = false;
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // ��������� ������������ �������������� ��������� ������������� ������ ��� ������������� ������� MainWindow
	RegWindow* reg = main->getRegWindow();
	reg->show();
	this->hide();
}

// ��� �������� ����� ����������� ������� �� ����������
void AuthWindow::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	qApp->quit(); // ������� �� ���������� ��� �������� ���� �����������
}