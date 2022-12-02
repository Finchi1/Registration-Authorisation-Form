#pragma once

#include <QMainWindow>
#include "ui_auth_window.h"

class AuthWindow : public QMainWindow
{
	Q_OBJECT

public:
	AuthWindow(QWidget* parent = nullptr);
	~AuthWindow();
public:
	QString& getName() { return m_name; }
	QString& getPass() { return m_password; }
	bool getAuthState() { return m_successful_auth; }
	void setAuthState(bool state = false) { m_successful_auth = state; } // устанавливает состояние авторизации
private:
	void nameEdit(QString text) { m_successful_auth = false; m_name = text; }
	void passEdit(QString text) { m_successful_auth = false; m_password = text; }
	void authButton(); // нажатие кнопки авторизации
	void regButton(); // нажатие кнопки регистрации
private:
	void closeEvent(QCloseEvent* event);
private:
	Ui::AuthWindowClass ui;
private:
	QString m_name;
	QString m_password;
	QWidget* m_parent; // указатель на родительский класс (MainWindow)
	bool m_successful_auth; // состояние авторизации
};
