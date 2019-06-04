#ifndef WIDGET_H
#define WIDGET_H
#include <QAbstractSocket>
#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include <QFileDialog>

namespace Ui {
class widget;
}

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = nullptr);
    ~widget();
    Ui::widget *ui;
    QTcpSocket *m_tcpClient;
    QFile *m_localFile;
    qint64 m_totalBytes;
    qint64 m_bytesWritten;
    qint64 m_bytesToWrite;
    qint64 m_payloadSize;
    QString m_fileName;
    QByteArray m_outBlock;

private slots:
    void openFile();
    void send();
    void startTransfer();
    void updateClientProgress(qint64);
    void displayError(QAbstractSocket::SocketError);
    void on_sendButton_clicked();
    void on_openButton_clicked();
};

#endif // WIDGET_H

