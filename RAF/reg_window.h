#pragma once

#include <QMainWindow>
#include "ui_reg_window.h"

class RegWindow : public QMainWindow
{
	Q_OBJECT

public:
	RegWindow(QWidget *parent = nullptr);
	~RegWindow();
public:
	QString& getName() { return m_name; }
	QString& getPass() { return m_password; }
	QString& getPassConf() { return m_password_confirm; }
private:
	void nameEdit(QString text) { m_name = text; }
	void passEdit(QString text) { m_password = text; }
	void passConfEdit(QString text) { m_password_confirm = text; }
	void authButton(); // нажатие кнопки авторизации
	void regButton(); // нажатие кнопки регистрации
private:
	void closeEvent(QCloseEvent* event);
private:
	Ui::RegWindowClass ui;
private:
	QString m_name;
	QString m_password;
	QString m_password_confirm;
	QWidget* m_parent; // указатель на родительский класс (MainWindow)
};
