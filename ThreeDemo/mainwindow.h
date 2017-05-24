#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>
#include <QLabel>
#include <string.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool m_status;
    int m_port;
    QHostAddress *m_pServerIP;
    QTcpSocket *m_pTcpSocket;
    QSerialPort *m_pserialPort;
    QTcpServer *m_pTcpServer;

public slots:
    void sendMessages();
    void dealMessage();
    void updateData();

private slots:
    void on_pushButtonOpenPort_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};





#endif // MAINWINDOW_H
