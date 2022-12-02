#pragma once

#include <QtWidgets/QMainWindow>
#include <QtSql/QtSql>
#include "ui_main_window.h"
#include "auth_window.h"
#include "reg_window.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindowClass ui;
private:
    void drawMainWindowText(QPainter* painter); // рисует текст главного окна
    bool connectDB(); // подключаем и открываем базу данных
    bool createTableDB(bool is_db_opened); // создаем разметку базы данных
private:
    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
private:
    QString m_UAtext; // текст "Главное окно"
    QFont m_TNR45; // шрифт для текста "Главное окно"
private:
    QString m_db_input; // строка для записи запросов в базу данных
    QSqlDatabase m_db; // база данных
private:
    AuthWindow* m_auth; // окно авторизации
    RegWindow* m_reg; // окно регистрации
public:
    AuthWindow* getAuthWindow() { return m_auth; }
    RegWindow* getRegWindow() { return m_reg; }
    void authorisationUser(); // обработчик события авторизации
    void registrationUser(); // обработчик события регистрации
};
