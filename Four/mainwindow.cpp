#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->tcp_server = new QTcpServer(this);
    this->tcp_client=new QTcpSocket(this);
    client_num=1;
    openAction = new QAction(QIcon(":/images/doc-open"), tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);
    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);
    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    statusBar();
    standardItemModel = new QStandardItemModel(this);
    ui->listView->setModel(standardItemModel);
    ui->listView->setFixedSize(100,300);
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setresize(int w,int h)
{
    ui->textEdit_console->setText(QString::number(w,10)+QString::number(h,10));
    this->h=h;this->w=w;
    ui->centralWidget->resize(w,h);
}
void MainWindow::debug(QString str)
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss:");
    consoleStr=consoleStr+"\n"+time+str;
    ui->textEdit_console->setText(consoleStr);
    ui->textEdit_console->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}
void MainWindow::open()
{


}
void MainWindow:: paintEvent(QPaintEvent *)
{
    /*QPainter painter(this);
    QPixmap pix;
    pix.load("c:/aa6.jpg");
    painter.translate(100,100);
    painter.rotate(movie_step*3.6);
    painter.translate(0,0);
    painter.drawPixmap(0, 0, 200, 200, pix);*/
    int Ox=10,Oy=520,Ow=801,Oh=281,Zx=Ox+20,Zy=Oy+Oh-20,Lx,Ly,Rx,Ry;
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

void MainWindow::data_timer_tick()
{
    speed[move]=qrand()%200;
    temp[move]=27+qrand()%40;
    for(int i=0;i<move;i++)
    {
        speed[i]=speed[i+1];
        temp[i]=temp[i+1];
    }
}
void MainWindow::ani_timer_tick()
{
  this->update();
  //ui->lineEdit->setText("温度："+QString::number(temp[move],10)+"；风速"+QString::number(speed[move],10));
  //ui->lineEdit->setText("温度："+QString::number(temp[move],10)+"；风速"+QString::number(speed[move],10));
}
void MainWindow::readyread()
{
     debug("接受到服务端数据");
     QString carr=QString::fromLocal8Bit(this->tcp_client->readAll());
     ui->textEdit_ca->setText(carr);
}
void MainWindow::connected()
{
    //debug("连接服务端成功");
    connect(this->tcp_client,SIGNAL(readyRead()),this,SLOT(readyread()));
}
void MainWindow::ReceiveData()
{
    debug("接受到客户端数据");
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client==client_socket){
            QString sarr=QString::fromLocal8Bit(this->client->readAll());
            ui->textEdit_sa->setText(sarr);
        }
    }

}

//连接服务端
void MainWindow::on_connectToSever_clicked()
{
    QString address=ui->lineEdit_ca->text();
    QString sport=ui->lineEdit_cp->text();
    if(QString(address).isEmpty()&& QString(sport).isEmpty()){ui->lineEdit_ca->setText("192.168.31.221");address="192.168.31.221"; ui->lineEdit_cp->setText("8080");sport="8080";}
    quint16 port=quint16(sport.toUInt());
    if(ui->connectToSever->text()=="连接服务端")
    {  
        this->tcp_client->connectToHost(address,port,QTcpSocket::ReadWrite);
        connect(this->tcp_client,SIGNAL(connected()),this,SLOT(connected()));
        //connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotErr(QAbstractSocket::SocketError)));
        ui->connectToSever->setText("断开连接");
        debug("连接服务端");
    }
    else
    {
        this->tcp_client->disconnectFromHost();
        //ui->connectToSever->setText("连接服务端");
        debug("断开连接");
    }
}
//连接到客户端
void MainWindow::newConnection()
{
    client=tcp_server->nextPendingConnection();
    QStandardItem *item = new QStandardItem("客户端"+QString::number(client_num,10));
    standardItemModel->appendRow(item);
    client_num++;
    connect(client,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    debug("连接到一个客户端");
    this->ClientSocketList.append(client);
    //connect(tcpSocket,SIGNAL(disconnected()),tcpSocket,SLOT(deleteLater()));
}
//监听端口
void MainWindow::on_connectToClient_clicked()
{
    QString sport=ui->lineEdit_cp->text();
    if(QString(sport).isEmpty()){ui->lineEdit_sp->setText("8080");sport="8080";}
    quint16 port=quint16(sport.toUInt());
    if(ui->connectToClient->text()=="监听端口")
    {
        tcp_server->listen(QHostAddress::Any,port);
        connect(tcp_server,SIGNAL(newConnection()),this,SLOT(newConnection()));
        //debug("监听端口"+sport+"中");
    //    ui->connectToClient->setText("停止监听");
    }
    else
    {
        this->tcp_server->close();
        ui->connectToClient->setText("监听端口");
        debug("已停止监听");
    }
}

void MainWindow::on_c_send_clicked()
{
     QString str=ui->textEdit_cs->toPlainText();
     QByteArray ba=str.toLocal8Bit();
     this->tcp_client->write(ba);
}

void MainWindow::on_s_send_clicked()
{
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client==client_socket){
        this->client->write(ui->textEdit_ss->toPlainText().toLocal8Bit());
        }
    }

}

void MainWindow::on_c_senddata_clicked()
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

void MainWindow::on_animation_clicked()
{
    ani_timer=new QTimer(this);
    connect(ani_timer, SIGNAL(timeout()), this, SLOT(ani_timer_tick()));
    ani_timer->setInterval(1000);
    ani_timer->start(100);  
}

void MainWindow::on_c_stopsend_clicked()
{
    this->data_timer->stop();
    this->ani_timer->stop();
}
void MainWindow::itemClicked(QModelIndex index)
{
   client=ClientSocketList.at(index.row());
   debug("与"+index.data().toString()+"通信中"+QString::number(index.row(),10));
}

void MainWindow::on_new_client_clicked()
{
    client_window= new Client_Window();
    client_window->setWindowTitle("客户端"+QString::number(client_num,10));
    client_window->show();
}
