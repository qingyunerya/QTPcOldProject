#include "widget2.h"
#include "ui_widget2.h"

widget2::widget2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget2)
{
    ui->setupUi(this);
    connect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

widget2::~widget2()
{
    delete ui;
}
void widget2::start(){
    if(!m_tcpServer.listen(QHostAddress::Any,8080)){
        qDebug()<<m_tcpServer.errorString();
        close();
        return;
    }
    ui->startButton->setEnabled(false);
    m_totalBytes=0;
    m_bytesReceived=0;
    m_fileNameSize=0;
    ui->serverStatusLabel->setText("监听");
    ui->serverProgressBar->reset();
}

void widget2::acceptConnection(){
    m_tcpServerConnection=m_tcpServer.nextPendingConnection();
    connect(m_tcpServerConnection,SIGNAL(readyRead()),this,SLOT(updateServerProgress()));
    connect(m_tcpServerConnection,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    ui->serverStatusLabel->setText("接受连接");
    //关闭服务器不再监听，直接进入数据收发
    m_tcpServer.close();
}

void widget2::updateServerProgress(){
    QDataStream in(m_tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_7);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构
    if (m_bytesReceived<=sizeof(qint64)*2){
        if((m_tcpServerConnection->bytesAvailable()>=sizeof(qint64)*2)&&(m_fileNameSize==0)){
            // 接收数据总大小信息和文件名大小信息
            in>>m_totalBytes>>m_fileNameSize;
            m_bytesReceived +=sizeof(qint64)*2;
        }
        if((m_tcpServerConnection->bytesAvailable()>=m_fileNameSize)&&(m_fileNameSize!=0)){
            // 接收文件名，并建立文件
            in>>m_fileName;
            ui->serverStatusLabel->setText(tr("接收文件 %1 …").arg(m_fileName));
            m_bytesReceived+=m_fileNameSize;
            m_localFile = new QFile(m_fileName);
            if (!m_localFile->open(QFile::WriteOnly)){
                qDebug() << "server: open file error!";
                return;
            }
        }
        else{
            return;
        }
    }
    // 如果接收的数据小于总数据，那么写入文件
    if(m_bytesReceived<m_totalBytes) {
        m_bytesReceived+=m_tcpServerConnection->bytesAvailable();
        m_inBlock = m_tcpServerConnection->readAll();
        m_localFile->write(m_inBlock);
        m_inBlock.resize(0);
    }
    ui->serverProgressBar->setMaximum(m_totalBytes);
    ui->serverProgressBar->setValue(m_bytesReceived);

    // 接收数据完成时
    if (m_bytesReceived==m_totalBytes){
        m_tcpServerConnection->close();
        m_localFile->close();
        ui->startButton->setEnabled(true);
        ui->serverStatusLabel->setText(tr("接收文件 %1 成功！").arg(m_fileName));
    }
}

void widget2::displayError(QAbstractSocket::SocketError socketError){
    Q_UNUSED(socketError)
    qDebug()<<m_tcpServerConnection->errorString();
    m_tcpServerConnection->close();
    ui->serverProgressBar->reset();
    ui->serverStatusLabel->setText("服务端就绪");
    ui->startButton->setEnabled(true);
}

void widget2::on_startButton_clicked()
{
    start();
}
