#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <client_window.h>
#include <QUdpSocket>
#include <widget.h>
#include <widget2.h>
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
    widget *client_widget;
    widget2 *sever_widget;
    QList<QTcpSocket *> ClientSocketList;
    QTcpSocket *tcp_client;
    QUdpSocket *uSocket;
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
private slots:
    void data_timer_tick();
    void ani_timer_tick();
    void readyread();
    void connected();
    void newConnection();
    void ReceiveData();
    void udp_receive();
    void on_connectToSever_clicked();
    void on_c_send_clicked();
    void on_connectToClient_clicked();
    void on_s_send_clicked();
    void on_c_senddata_clicked();
    void on_animation_clicked();
    void on_c_stopsend_clicked();
    void itemClicked(QModelIndex index);
    void on_new_client_clicked();
    void on_file_trasport_clicked();
    void on_file_trasport_sever_2_clicked();
    void on_udp_sever_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
