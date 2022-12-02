#include "auth_window.h"
#include "reg_window.h"
#include "main_window.h"

AuthWindow::AuthWindow(QWidget *parent)
	: QMainWindow(parent), m_parent(parent), m_successful_auth(false)
{
	ui.setupUi(this);
	this->setFixedSize(this->size()); // устанавливем фиксированный размер окна
	// Соединяем события редактированя полей и нажатия кнопок с обработчиками
	connect(ui.nameEdit, &QLineEdit::textEdited, this, &AuthWindow::nameEdit);
	connect(ui.passEdit, &QLineEdit::textEdited, this, &AuthWindow::passEdit);
	connect(ui.authButton, &QPushButton::clicked, this, &AuthWindow::authButton);
	connect(ui.regButton, &QPushButton::clicked, this, &AuthWindow::regButton);
}

AuthWindow::~AuthWindow()
{}

// При нажатии на кнопку авторизации, запускаем соответствующий метод класса MainWindow
void AuthWindow::authButton()
{
	m_successful_auth = false;
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // выполняем динамическое преобразования указателя родительского класса для использования методов MainWindow
	main->authorisationUser();
}

// При нажатии на кнопку регистрации показываем форму регистрации, а форму авторизации скрываем
void AuthWindow::regButton()
{
	m_successful_auth = false;
	MainWindow* main = dynamic_cast<MainWindow*>(m_parent); // выполняем динамическое преобразования указателя родительского класса для использования методов MainWindow
	RegWindow* reg = main->getRegWindow();
	reg->show();
	this->hide();
}

// При закрытии формы авторизации выходим из приложения
void AuthWindow::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	qApp->quit(); // выходим из приложения при закрытии окна авторизации
}