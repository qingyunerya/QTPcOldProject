#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H
#include <QTcpSocket>
#include <QMainWindow>
#include <QTimer>
#include <QTime>
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
    QTcpSocket *tcp_client;
    QString consoleStr;
    private slots:
    void on_c_send_clicked();
    void readyread();
    void connected();
    void disconnected();
    void on_connectToSever_clicked();

private:
    Ui::Client_Window *ui;
};

#endif // CLIENT_WINDOW_H
