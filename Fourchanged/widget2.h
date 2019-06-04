#ifndef WIDGET2_H
#define WIDGET2_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QTcpServer>
namespace Ui {
class widget2;
}

class widget2 : public QWidget
{
    Q_OBJECT

public:
    explicit widget2(QWidget *parent = nullptr);
    ~widget2();
    QTcpServer m_tcpServer;
    QTcpSocket *m_tcpServerConnection;
    qint64 m_totalBytes;
    qint64 m_bytesReceived;
    qint64 m_fileNameSize;
    QString m_fileName;
    QFile *m_localFile;
    QByteArray m_inBlock;
private slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);
    void on_startButton_clicked();

private:
    Ui::widget2 *ui;
};

#endif // WIDGET2_H
