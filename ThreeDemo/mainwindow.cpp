#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_status = false;
    this->setFixedSize(652,337);
    this->setWindowTitle(tr("Three"));
    m_port = 8878;
    m_pServerIP = new QHostAddress();
    m_pserialPort = new QSerialPort();
    m_pTcpServer = new QTcpServer(this);
    if(!m_pTcpServer->listen(QHostAddress::Any,m_port))
    {
        QMessageBox::information(this,tr("error"),tr("listenerror!"));
        return;
    }
    else
    {
        connect(m_pTcpServer, SIGNAL(newConnection()), this, SLOT(sendMessages()));
    }
    connect(m_pserialPort,SIGNAL(readyRead()),this,SLOT(updateData()));
    ui->pushButtonSend->setEnabled(false);
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOpenPort_clicked()
{
    if(ui->pushButtonOpenPort->text()=="OpenPort")
    {
        m_pserialPort->setPortName(ui->lineEdit->text());
        m_pserialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
        m_pserialPort->setDataBits(QSerialPort::Data8);		//数据位为8位
        m_pserialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
        m_pserialPort->setParity(QSerialPort::NoParity);	//无校验位
        m_pserialPort->setStopBits(QSerialPort::OneStop);	//一位停止位
        m_pserialPort->close();					//先关串口，再打开，可以保证串口不被其它函数占用。
        if(m_pserialPort->open(QIODevice::ReadWrite))		//以可读写的方式打开串口
        {
            connect(m_pserialPort,SIGNAL(readyRead()),this,SLOT(read_Com()));
            ui->pushButtonOpenPort->setText("ClosePort");
        }
        else
        {
            QMessageBox::information(this,tr("error"),tr("com name error!"));
            return;
        }
    }
    else
    {
        ui->pushButtonOpenPort->setText("OpenPort");
        m_pserialPort->close();
    }
    return;
}

void MainWindow::sendMessages()
{
    m_pTcpSocket = m_pTcpServer->nextPendingConnection();
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(dealMessage()));
}

void MainWindow::dealMessage()
{//receive from network and send to serialport
    QString str="";
    QByteArray datagram;
    char *data="";
    while(m_pTcpSocket->bytesAvailable()>0)
    {
        datagram.resize(m_pTcpSocket->bytesAvailable());
        m_pTcpSocket->read(datagram.data(),datagram.size());
        data=datagram.data();
        str = QString::fromUtf8(data);
        ui->recvTextBrowser->append(str);
    }
    m_pserialPort->write(data);
    return;
}

void MainWindow::updateData()
{//receieve from serialport and send to network
    QByteArray requestData = m_pserialPort->readAll();
    QString str = "";
    if(requestData.size()>0)
    {
        str = requestData.data();
        ui->sendTextBrowser->append(str);
    }
    m_pTcpSocket->write(str.toLatin1());
    //sentToNetwork
    return;
}

void MainWindow::on_pushButton_clicked()
{
    ui->recvTextBrowser->clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->sendTextBrowser->clear();
}
