#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H
#include <QTcpSocket>
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QUdpSocket>

namespace Ui {
class Client_Window;
}

class Client_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client_Window(QWidget *parent = nullptr);
    ~Client_Window();
    void debug(QString str);
    void paintEvent(QPaintEvent *);
    QTcpSocket *tcp_client;
    QString consoleStr;
    QTimer *data_timer;
    QTimer *ani_timer;
    int temp[150];
    int speed[150];
    int move;
    int index;
    private slots:
    void data_timer_tick();
    void ani_timer_tick();
    void on_c_senddata_clicked();
    void on_animation_clicked();
    void on_c_stopsend_clicked();
    void on_c_send_clicked();
    void readyread();
    void connected();
    void disconnected();
    void on_connectToSever_clicked();

    void on_udp_client_clicked();

private:
    Ui::Client_Window *ui;
};

#endif // CLIENT_WINDOW_H
