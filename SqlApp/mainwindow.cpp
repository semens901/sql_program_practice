#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = std::make_unique<DataBase>("QMARIADB");
    ui->le_host_name->setText("127.0.0.1");
}

void MainWindow::connect()
{
    if((ui->le_host_name->text().size() != 0) &&
        (ui->le_databasename->text().size() != 0) &&
       (ui->le_user_name->text().size() != 0) &&
       (ui->le_password->text().size() != 0))
    {
        db.get()->set_host_name(ui->le_host_name->text());
        db.get()->set_database(ui->le_databasename->text());
        db.get()->set_user_name(ui->le_user_name->text());
        db.get()->set_password(ui->le_password->text());

        if(db.get()->open())
        {
            qDebug() << "Connected";
            ui->stackedWidget->setCurrentIndex(1);
            db.get()->exec_query("CREATE TABLE TelephoneBook(Firstname TEXT, Lastname TEXT, Telephone INT);");
            db.get()->set_sql_table_model(this);
            db.get()->set_table("TelephoneBook");
            db.get()->model_select();
            ui->tableView->setModel(db.get()->get_module());
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Failed to connect");
            msgBox.exec();
        }
    }
}

void MainWindow::add()
{
    db.get()->add_row();
}

void MainWindow::delete_element()
{
    db.get()->remove_row(db.get()->get_row());
}

MainWindow::~MainWindow()
{
    delete ui;
}

DataBase::DataBase()
{
    db = QSqlDatabase::addDatabase("QMARIADB");
    initialize_query();
}

DataBase::DataBase(const QString& base_type)
{
    db = QSqlDatabase::addDatabase(base_type);
    initialize_query();
}

bool DataBase::open()
{
    if(db.open())
    {
        qDebug() << "Connected";
        return true;
    }
    else
    {
        qDebug() << "Failed to connect";
        return false;
    }
}

void DataBase::set_host_name(const QString& host_name)
{
    db.setHostName(host_name);
}

void DataBase::set_user_name(const QString& user_name)
{
    db.setUserName(user_name);
}

void DataBase::set_password(const QString& passwd)
{
    db.setPassword(passwd);
}

void DataBase::initialize_query()
{
    query = new QSqlQuery(db);
}

bool DataBase::exec_query(const QString& query)
{
    return this->query->exec(query);
}

void DataBase::set_database(const QString& db_name)
{
    db.setDatabaseName(db_name);
}

void DataBase::set_sql_table_model(MainWindow* cls)
{
    db_model = new QSqlTableModel(cls, db);
}

void DataBase::set_table(const QString& table_name)
{
    db_model->setTable(table_name);
}

void DataBase::model_select()
{
    db_model->select();
}

QSqlTableModel* DataBase::get_module()
{
    return db_model;
}

void DataBase::add_row()
{
    db_model->insertRow(db_model->rowCount());
}

void DataBase::set_row(size_t row)
{
    this->row = row;
}

size_t DataBase::get_row()
{
    return row;
}

void DataBase::remove_row(size_t row)
{
    db_model->removeRow(row);
}

DataBase::~DataBase()
{

}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    db.get()->set_row(index.row());
}

