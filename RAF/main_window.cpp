#include "main_window.h"
#include <qfontmetrics.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qdebug.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.mainLabel->hide(); // прячем label с текстом украинской локализации "Главное окно"
	m_UAtext = ui.mainLabel->text(); // записываем текст из label в переменную
	m_TNR45 = QFont("Times New Roman", 45); // задаем шрифт текста основного окна
	QFontMetrics TNR45_metrics(m_TNR45); // измеритель шрифта для установки минимального размера главного окна
	// Устанавливаем минимальный размер главного окна
	ui.centralWidget->setMinimumWidth(TNR45_metrics.horizontalAdvance(m_UAtext));
	ui.centralWidget->setMinimumHeight(TNR45_metrics.height());
	connectDB(); // подключаем базу данных
	createTableDB(m_db.isOpen()); // создаем разметку базы данных
	m_auth = new AuthWindow(this); // создаем окно авторизации
	m_reg = new RegWindow(this); // создаем окно регистрации
	// По умолчанию отображается только окно авторизации
	this->hide();
	m_auth->show(); 
	m_reg->hide();
}

MainWindow::~MainWindow()
{
	// Если была успешная авторизация и база данных открыта,
	// то обновляем в базе данных для авторизированного пользователя значения ширины, высоты и координаты положения основного окна
	if (m_auth->getAuthState() && m_db.isOpen())
	{
		m_db_input = QString(
			" UPDATE userlist "
			" SET x = %2, y = %3, width = %4, height = %5 "
			" WHERE name = '%1' "
		).arg(m_auth->getName()).arg(this->x()).arg(this->y()).arg(this->width()).arg(this->height());
		QSqlQuery query;
		if (!query.exec(m_db_input))
			qDebug() << "MainWindow::~MainWindow() - Wrong query - " << query.lastError();
	}
	// Закрываем базу данных и очищаем память
	m_db.close();
	delete m_auth;
	delete m_reg;
}

// Рисуем текст главного окна
void MainWindow::drawMainWindowText(QPainter* painter)
{
	int w = this->width();
	int h = this->height();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(QColor("red"), 2));
	painter->setBrush(QBrush(QColor("black")));
	QFontMetrics fmetrics(m_TNR45);
	int fmetrics_hadvance = fmetrics.horizontalAdvance(m_UAtext);
	int fmetrics_height = fmetrics.height();
	QPainterPath path;
	path.addText(w / 2.0 - fmetrics_hadvance / 2.0, h / 2.0, m_TNR45, m_UAtext);
	painter->drawPath(path);
}

// Подключаем базу данных и открываем ее
bool MainWindow::connectDB()
{
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName("authorisation.db");
	if (!m_db.open())
	{
		qDebug() << "MainWindow::connectDB - Can't open database authorisation - " << m_db.lastError();
		return false;
	}
	qDebug() << "MainWindow::connectDB - Database authorisation was opened";
	return true;
}

// Создаем разметку базу данных, если еще не создана
bool MainWindow::createTableDB(bool is_db_opened)
{
	if (!is_db_opened)
	{
		qDebug() << "MainWindow::createTableDB - Can't create a table in authorisation database - database is not opened";
		return false;
	}
	QSqlQuery query;
	m_db_input =
		"CREATE TABLE IF NOT EXISTS userlist ( "
		"number INTEGET PRIMARY KEY NOT NULL, "
		"name VARCHAR(20), "
		"password VARCHAR(20), "
		"x INTEGER, "
		"y INTEGER, "
		"width INTEGER, "
		"height INTEGER );";
	if (!query.exec(m_db_input))
	{
		qDebug() << "MainWindow::createTableDB - Can't create a table in authorisation database - " << query.lastError();
		return false;
	}
	qDebug() << "MainWindow::createTableDB - Table userlist was created in authorisation database";
	return true;
}

// Обработчик события paintEvent
void MainWindow::paintEvent(QPaintEvent* event)
{
	if (this->isHidden())
		return;
	Q_UNUSED(event);
	QPainter painter(this);
	drawMainWindowText(&painter); // рисуем текст главного окна
}

// Обработчик события showEvent
void MainWindow::showEvent(QShowEvent* event)
{
	// Если небыло успешной авторизации или база данных закрыта, то прекращаем выполнение функции
	if (!m_auth->getAuthState() || !m_db.isOpen())
		return;
	m_db_input = QString(
		" SELECT * FROM userlist "
		" WHERE name = '%1' "
	).arg(m_auth->getName());
	QSqlQuery query;
	QSqlRecord record;
	if (!query.exec(m_db_input)) 
	{
		qDebug() << "MainWindow::showEvent - Wrong query - " << query.lastError();
		return;
	}
	query.next(); // выбирем все поля базы данных для авторизированного пользователя 
	record = query.record(); // записываем полученые данные из базы данных в переменную
	// Приводим данные в удобный формат
	int x = record.field("x").value().toInt();
	int y = record.field("y").value().toInt();
	int w = record.field("width").value().toInt();
	int h = record.field("height").value().toInt();
	qDebug() << "MainWindow::showEvent - x = " << x << ", y = " << y << ", whidth = " << w << ", height = " << h;
	// Устанавливаем координаты и размер главного окна согласно данным в базе для авторизированного пользователя
	this->move(x, y);
	this->resize(w, h);
}

// Обработчик события авторизации
void MainWindow::authorisationUser()
{
	// Если база данных не открыта, то сразу выходим из функции
	if (!m_db.isOpen())
	{
		qDebug() << "MainWindow::registrationUser() - db is not opened";
		return;
	}
	// Получаем данные авторизации
	QString name = m_auth->getName();
	QString password = m_auth->getPass();
	QSqlQuery query;
	m_db_input = QString(
		" SELECT name, password FROM userlist "
		" WHERE name = '%1' AND password = '%2'; "
	).arg(name).arg(password);
	if (!query.exec(m_db_input))
		qDebug() << "MainWindow::registrationUser() - Wrong query - " << query.lastError();
	QMessageBox wrong_msgbox(this);
	// Проверяем соответствие имени пользователя и пароля записи в базе данных, если нет соответствия, 
	// то выводим QMessageBox с сообщением и прекращаем выполнения функции
	if (!query.next())
	{
		qDebug() << "MainWindow::registrationUser() - Wrong username or password";
		wrong_msgbox.setText("Wrong username or password");
		wrong_msgbox.setIcon(QMessageBox::Warning);
		wrong_msgbox.exec();
		return;
	}
	// Авторизация успешна
	m_auth->setAuthState(true); // передаем данные об успешной авторизации объекту формы авторизации
	m_auth->hide(); // скрываем окно авторизации
	this->show(); // показываем основное окно
}

// Обработчик события регистрации пользователя
void MainWindow::registrationUser()
{
	// Если база данных не открыта, то сразу выходим из функции
	if (!m_db.isOpen())
	{
		qDebug() << "MainWindow::registrationUser() - db is not opened";
		return;
	}
	// Получаем данные регистрации
	QString name = m_reg->getName();
	QString password = m_reg->getPass();
	QString password_confirm = m_reg->getPassConf();
	QSqlQuery query;
	QSqlRecord record;
	
	m_db_input = QString(
		" SELECT number FROM userlist; "
	);
	if (!query.exec(m_db_input))
		qDebug() << "MainWindow::registrationUser() -  Wrong query - " << query.lastError();
	query.last(); // получаем последний порядковый номер пользователя в базе данных
	record = query.record(); // записываем данные в переменную
	int last_number = record.field("number").value().toInt(); // приводим данные в удобную форму (int)
	qDebug() << "MainWindow::registrationUser() - Last number is " << last_number;
	m_db_input = QString(
		" SELECT name FROM userlist "
		" WHERE name = '%1'; "
	).arg(name);
	if (!query.exec(m_db_input))
		qDebug() << "MainWindow::registrationUser() -  Wrong query - " << query.lastError();
	query.next(); // ищем пользователя с указанным в регистрации именем в базе
	record = query.record(); // записываем данные в переменную
	QString temp_name = record.field("name").value().toString(); // приводим данные в удобную форму (Qstring)
	QMessageBox existed_msgbox(this);
	// Если пользователь с таким именем уже существует, то выводим QMessageBox и прекращаем выполнения функции
	if (name == temp_name)
	{
		qDebug() << "Username " << name << " already exist";
		QString existed_str("Username %1 already exist");
		existed_str = existed_str.arg(name);
		existed_msgbox.setText(existed_str);
		existed_msgbox.setIcon(QMessageBox::Warning);
		existed_msgbox.exec();
		return;
	}
	QMessageBox wrong_msgbox(this);
	// Если регистрация успешна, то записываем данные в базу данных и показываем только окно авторизации
	if (name.length() >= 4 && name.length() < 20 && password.length() >= 4 && password.length() < 20 && password == password_confirm)
	{
		m_db_input = QString(
			"INSERT INTO userlist (number, name, password, x, y, width, height)"
			"VALUES (%1, '%2', '%3', %4, %5, %6, %7)"
		).arg(++last_number).arg(name).arg(password).arg(this->x()).arg(this->y()).arg(this->width()).arg(this->height());
		if (!query.exec(m_db_input))
		{
			qDebug() << "MainWindow::registrationUser - Can't insert values in db";
			return;
		}
		m_reg->hide();
		m_auth->show();
		return;
	}
	// Если имя или пароль не соответсвует требованиям, то выводим QMessageBox и прекращаем выполнения функции
	else if (name.length() < 4 || name.length() >= 20)
	{
		qDebug() << "MainWindow::registrationUser - Username must has betwen 4 and 20 characters";
		wrong_msgbox.setText("Username must has betwen 4 and 20 characters");
		wrong_msgbox.setIcon(QMessageBox::Warning);
		wrong_msgbox.exec();
		return;
	}
	else if (password.length() < 4 || password.length() >= 20)
	{
		qDebug() << "MainWindow::registrationUser - Password must has betwen 4 and 20 characters";
		wrong_msgbox.setText("Password must has betwen 4 and 20 characters");
		wrong_msgbox.setIcon(QMessageBox::Warning);
		wrong_msgbox.exec();
		return;
	}
	else if (password != password_confirm)
	{
		qDebug() << "MainWindow::registrationUser - password != password_confirm";
		wrong_msgbox.setText("Password must be coinside with password confirm");
		wrong_msgbox.setIcon(QMessageBox::Warning);
		wrong_msgbox.exec();
		return;
	}
}