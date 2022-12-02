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
    void drawMainWindowText(QPainter* painter); // ������ ����� �������� ����
    bool connectDB(); // ���������� � ��������� ���� ������
    bool createTableDB(bool is_db_opened); // ������� �������� ���� ������
private:
    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
private:
    QString m_UAtext; // ����� "������� ����"
    QFont m_TNR45; // ����� ��� ������ "������� ����"
private:
    QString m_db_input; // ������ ��� ������ �������� � ���� ������
    QSqlDatabase m_db; // ���� ������
private:
    AuthWindow* m_auth; // ���� �����������
    RegWindow* m_reg; // ���� �����������
public:
    AuthWindow* getAuthWindow() { return m_auth; }
    RegWindow* getRegWindow() { return m_reg; }
    void authorisationUser(); // ���������� ������� �����������
    void registrationUser(); // ���������� ������� �����������
};
