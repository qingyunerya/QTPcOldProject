#include "widget.h"
#include "ui_widget.h"

widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);
    m_payloadSize=64*1024;
    m_totalBytes=0;
    m_bytesWritten=0;
    m_bytesToWrite=0;
    m_tcpClient=new QTcpSocket(this);
    connect(m_tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
    connect(m_tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    connect(m_tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}

widget::~widget()
{
    delete ui;
}
void widget::openFile(){
    m_fileName=QFileDialog::getOpenFileName(this);
    if(!m_fileName.isEmpty()){
        ui->sendButton->setEnabled(true);
        ui->clientStatusLabel->setText(QString("打开文件 %1 成功!").arg(m_fileName));
    }
}

void widget::send(){
    ui->sendButton->setEnabled(false);
    m_bytesWritten=0;
    ui->clientStatusLabel->setText("连接中...");
    m_tcpClient->connectToHost(ui->hostLineEdit->text(),ui->portLineEdit->text().toInt());
}

void widget::startTransfer(){
    m_localFile=new QFile(m_fileName);
    if(!m_localFile->open(QFile::ReadOnly)){
        qDebug()<<"client：open file error!";
        return;
    }
    m_totalBytes=m_localFile->size();
    QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_7);
    QString currentFileName=m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);
    //文件总大小、文件名大小、文件名
    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
    m_totalBytes+=m_outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<m_totalBytes<<qint64(m_outBlock.size()-sizeof(qint64)*2);
    m_bytesToWrite=m_totalBytes-m_tcpClient->write(m_outBlock);
    ui->clientStatusLabel->setText("已连接");
    m_outBlock.resize(0);
}

void widget::updateClientProgress(qint64 numBytes){
    m_bytesWritten+=(int)numBytes;
    if(m_bytesToWrite>0){
        m_outBlock=m_localFile->read(qMin(m_bytesToWrite,m_payloadSize));
        m_bytesToWrite-=(int)m_tcpClient->write(m_outBlock);
        m_outBlock.resize(0);
    }
    else{
        m_localFile->close();
    }
    ui->clientProgressBar->setMaximum(m_totalBytes);
    ui->clientProgressBar->setValue(m_bytesWritten);

    if(m_bytesWritten==m_totalBytes){
        ui->clientStatusLabel->setText(QString("传送文件 %1 成功").arg(m_fileName));
        m_localFile->close();
        m_tcpClient->close();
    }
}

void widget::displayError(QAbstractSocket::SocketError){
    qDebug()<<m_tcpClient->errorString();
    m_tcpClient->close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText("客户端就绪");
    ui->sendButton->setEnabled(true);
}

void widget::on_sendButton_clicked()
{
    send();
}

void widget::on_openButton_clicked()
{
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText("状态：等待打开文件！");
    openFile();
}
