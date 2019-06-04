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
void Client_Window:: paintEvent(QPaintEvent *)
{
    /*QPainter painter(this);
    QPixmap pix;
    pix.load("c:/aa6.jpg");
    painter.translate(100,100);
    painter.rotate(movie_step*3.6);
    painter.translate(0,0);
    painter.drawPixmap(0, 0, 200, 200, pix);*/
    int Ox=10,Oy=390,Ow=691,Oh=161,Zx=Ox+20,Zy=Oy+Oh-20,Lx,Ly,Rx,Ry;
    Lx=Zx-20;Ly=Zy+20;Rx=Zx+Ow-20;Ry=Ly-Oh+20;
    QPainter painter(this);
    painter.setPen(Qt::lightGray);
    painter.drawRect(Ox,Oy,Ow,Oh);
    QPen pen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine(Lx, Zy, Rx, Zy);
    painter.drawLine(Zx, Ry, Zx, Ly);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    if(index==1)
    {
        for(int i=0;i<move;i++)
        {
            painter.drawLine(i*5+Zx,Zy-speed[i],i*5+5+Zx,Zy-speed[i+1]);
        }
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        for(int i=0;i<move;i++)
        {
            painter.drawLine(i*5+Zx,Zy-temp[i],i*5+5+Zx,Zy-temp[i+1]);
        }
    }
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
void Client_Window::data_timer_tick()
{
    speed[move]=qrand()%100;
    temp[move]=27+qrand()%20;
    for(int i=0;i<move;i++)
    {
        speed[i]=speed[i+1];
        temp[i]=temp[i+1];
    }
}
void Client_Window::ani_timer_tick()
{
  this->update();
  ui->lineEdit->setText("温度："+QString::number(temp[move],10)+"；风速："+QString::number(speed[move],10));
}
void Client_Window::on_c_senddata_clicked()
{
    move=149;
    for(int i=0;i<move+1;i++)
    {
        temp[i]=0;
        speed[i]=0;
    }
    index=1;
    data_timer=new QTimer(this);
    connect(data_timer, SIGNAL(timeout()), this, SLOT(data_timer_tick()));
    data_timer->setInterval(0);
    data_timer->start(100);
}

void Client_Window::on_animation_clicked()
{
    ani_timer=new QTimer(this);
    connect(ani_timer, SIGNAL(timeout()), this, SLOT(ani_timer_tick()));
    ani_timer->setInterval(1000);
    ani_timer->start(100);
}

void Client_Window::on_c_stopsend_clicked()
{
    this->data_timer->stop();
    this->ani_timer->stop();
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

void Client_Window::on_udp_client_clicked()
{
    QUdpSocket qus;
    QString address=ui->lineEdit_ca->text();
    QString sport=ui->lineEdit_cp->text();
    if(QString(address).isEmpty()&& QString(sport).isEmpty()){ui->lineEdit_ca->setText("192.168.31.221");address="192.168.31.221"; ui->lineEdit_cp->setText("8080");sport="8080";}
    quint16 port=quint16(sport.toUInt());
    qus.writeDatagram("msg", QHostAddress(address), port);
}
