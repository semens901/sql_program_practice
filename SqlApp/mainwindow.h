#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QMessageBox>
#include<QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class DataBase;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<DataBase> db;
public slots:
    void connect();
    void add();
    void delete_element();
private slots:
    void on_tableView_clicked(const QModelIndex &index);
};

class DataBase
{
public:
    DataBase();
    DataBase(const QString& base_type);
    ~DataBase();

    void set_host_name(const QString& host_name);
    void set_user_name(const QString& user_name);
    void set_password(const QString& passwd);
    void set_database(const QString& db_name);
    void set_sql_table_model(MainWindow* cls);
    void set_table(const QString& table_name);
    void model_select();
    void add_row();
    void set_row(size_t row);
    void remove_row(size_t row);
    size_t get_row();
    QSqlTableModel* get_module(); // Осторожно, возвращает неконстантный указатель на объект!

    bool open(); // Вызывать после конструктора для открытия бд
    void initialize_query();
    bool exec_query(const QString& query);



private:
    QSqlDatabase db;
    QSqlQuery* query;
    QSqlTableModel* db_model;
    size_t row;
};

#endif // MAINWINDOW_H
