#include "reg_window.h"
#include "main_window.h"
#include "auth_window.h"

RegWindow::RegWindow(QWidget *parent)
	: QMainWindow(parent), m_parent(parent)
{
	ui.setupUi(this);
	this->setFixedSize(this->size()); // устанавливем фиксированный размер окна
	// Соединяем события редактированя полей и нажатия кнопок с обработчиками
	connect(ui.nameEdit, &QLineEdit::textEdited, this, &RegWindow::nameEdit);
	connect(ui.passEdit, &QLineEdit::textEdited, this, &RegWindow::passEdit);
	connect(ui.passConfEdit, &QLineEdit::textEdited, this, &RegWindow::passConfEdit);
	connect(ui.regButton, &QPushButton::clicked, this, &RegWindow::regButton);
	connect(ui.authButton, &QPushButton::clicked, this, &RegWindow::authButton);
}

RegWindow::~RegWindow()
{}

// При нажатии на кнопку авторизации показываем форму авторизации, а форму регистрации скрываем
void RegWindow::authButton()
{
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // выполняем динамическое преобразования указателя родительского класса для использования методов MainWindow
	AuthWindow* auth = main->getAuthWindow();
	auth->show();
	this->hide();
}

// При нажатии на кнопку регистрации, запускаем соответствующий метод класса MainWindow
void RegWindow::regButton()
{
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // выполняем динамическое преобразования указателя родительского класса для использования методов MainWindow
	main->registrationUser();
}

// При закрытии формы регистрации показываем форму авторизации
void RegWindow::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // выполняем динамическое преобразования указателя родительского класса для использования методов MainWindow
	AuthWindow* auth = main->getAuthWindow();
	auth->show();
}