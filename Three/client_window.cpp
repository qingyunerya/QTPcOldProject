#include "client_window.h"
#include "ui_client_window.h"

Client_Window::Client_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client_Window)
{
    ui->setupUi(this);
    this->tcp_client=new QTcpSocket(this);
}

Client_Window::~Client_Window()
{
    delete ui;
}
void Client_Window::debug(QString str)
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss:");
    consoleStr=consoleStr+"\n"+time+str;
    ui->textEdit_console->setText(consoleStr);
    ui->textEdit_console->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}
void Client_Window::on_connectToSever_clicked()
{
    QString address=ui->lineEdit_ca->text();
    QString sport=ui->lineEdit_cp->text();
    if(QString(address).isEmpty()&& QString(sport).isEmpty()){ui->lineEdit_ca->setText("192.168.31.221");address="192.168.31.221"; ui->lineEdit_cp->setText("8080");sport="8080";}
    quint16 port=quint16(sport.toUInt());
    if(ui->connectToSever->text()=="连接服务端")
    {
        this->tcp_client->connectToHost(address,port,QTcpSocket::ReadWrite);
        connect(this->tcp_client,SIGNAL(connected()),this,SLOT(connected()));
        connect(this->tcp_client,SIGNAL(disconnected()),this,SLOT(disconnected()));
        //connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotErr(QAbstractSocket::SocketError)));
        ui->connectToSever->setText("断开连接");
        debug("连接服务端中");
    }
    else
    {
        this->tcp_client->disconnectFromHost();
        ui->connectToSever->setText("连接服务端");
        debug("断开连接");
    }
}
void Client_Window::on_c_send_clicked()
{
     QString str=this->windowTitle()+":"+ui->textEdit_cs->toPlainText();
     QByteArray ba=str.toLocal8Bit();
     this->tcp_client->write(ba);
}
void Client_Window::readyread()
{
     debug("接受到服务端数据");
     QString carr=QString::fromLocal8Bit(this->tcp_client->readAll());
     ui->textEdit_ca->setText(carr);
}
void Client_Window::connected()
{
    debug("连接服务端成功");
    connect(this->tcp_client,SIGNAL(readyRead()),this,SLOT(readyread()));
}
void Client_Window::disconnected()
{
    //this->tcp_client->disconnectFromHost();
    ui->connectToSever->setText("连接服务端");
    debug("服务端已断开连接");
}
