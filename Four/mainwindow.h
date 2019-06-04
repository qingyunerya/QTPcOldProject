#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <client_window.h>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>
#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QAction>
#include <QStandardItemModel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void debug(QString str);
    void open();
    Client_Window *client_window;
    QList<QTcpSocket *> ClientSocketList;
    QTcpSocket *tcp_client;
    QStandardItemModel *standardItemModel;
    QTcpServer *tcp_server;
    QTcpSocket *client;
    QTimer *data_timer;
    QTimer *ani_timer;
    QAction *openAction;
    QString consoleStr;
    int client_num;
    int temp[150];
    int speed[150];
    int move;
    int index;
    int w=0,h=0;
    void setresize(int w=0,int h=0);
private slots:
    void data_timer_tick();
    void ani_timer_tick();
    void readyread();
    void connected();
    void newConnection();
    void ReceiveData();
    void on_connectToSever_clicked();
    void on_c_send_clicked();
    void on_connectToClient_clicked();
    void on_s_send_clicked();
    void on_c_senddata_clicked();
    void on_animation_clicked();
    void on_c_stopsend_clicked();
    void itemClicked(QModelIndex index);
    void on_new_client_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
