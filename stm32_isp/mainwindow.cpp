#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSerialPort"
#include "QDebug"
#include "QSerialPortInfo"
#include "QAction"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool started = false;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QString menu_temp = info.portName() + ":" + info.description();
        QAction *action = new QAction(menu_temp,this);
        if(started ==  false)
        {
            port = menuBar()->addMenu("PORT:" + info.portName());
            current_port = info.portName();
            started = true;
        }
        port->addAction(action);
    }
    connect(port,SIGNAL(triggered(QAction*)),this,SLOT(port_menu_click(QAction*)));
    QStringList Baudrate;
    Baudrate.append("460800");
    Baudrate.append("256000");
    Baudrate.append("230400");
    Baudrate.append("128000");
    Baudrate.append("115200");
    Baudrate.append("76800");
    Baudrate.append("57600");
    Baudrate.append("38400");
    Baudrate.append("19200");
    Baudrate.append("14400");
    Baudrate.append("9600");
    baudrate_menu = menuBar()->addMenu("bsp:" + Baudrate.at(0));
    for(QString baud:Baudrate)
    {
        QAction *action = new QAction(baud,this);
        baudrate_menu->addAction(action);
//        qDebug()<<baud;
    }
   connect(baudrate_menu,SIGNAL(triggered(QAction*)),this,SLOT(baud_menu_click(QAction*)));

   current_baudrate = Baudrate.at(0).toInt();
   serial_port_init();
   connect(&serial, &QSerialPort::readyRead, this,&MainWindow::serial_read_dat);
}


void MainWindow::serial_read_dat(void)
{
    qDebug()<<serial.readAll();
//    qDebug()<<
}


void MainWindow::port_menu_click(QAction* act)
{
//    qDebug()<<"Port action click: "<<act->text();
    for(QAction *action:port->actions())
    {
        action->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/none.png")));

//        qDebug()<<action->text();
    }
    current_port = act->text().split(":").at(0);
    act->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/select.png")));
    port->setTitle("PORT : " + act->text().split(":").at(0));
}


void MainWindow::baud_menu_click(QAction* act)
{
    qDebug()<<act->text();
    current_baudrate = act->text().toInt();
    for(QAction *action:baudrate_menu->actions())
    {
        action->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/none.png")));

    }
    act->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/select.png")));
    baudrate_menu->setTitle("bps:" + act->text());
    serial.setBaudRate(current_baudrate);
}


void MainWindow::serial_port_init(void)
{

    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);

    serial.setPortName(current_port);
    serial.setBaudRate(current_baudrate);
    qDebug()<<port->title().split(":").at(1);
    qDebug()<<baudrate_menu->title().split(":").at(1);
}


MainWindow::~MainWindow()
{
    serial.close();
    delete ui;
}

void MainWindow::serial_open(void)
{
    if(serial.isOpen() == false || serial.portName() != current_port || serial.baudRate() != current_baudrate)
    {
        serial.close();
        serial.setPortName(current_port);
        serial.setBaudRate(current_baudrate);
        if (!serial.open(QIODevice::ReadWrite)) {
            qDebug()<<"Open "<<current_port<<" faile";
            return;
        }
    }
}
void MainWindow::on_pushButton_clicked()
{
    serial_open();
    char start = 0x7f;
    serial.write(&start,1);
}

void MainWindow::on_pushButton_2_clicked()
{
    serial.close();
}

void MainWindow::on_pushButton_3_clicked()
{
    serial_open();
    uint8_t start[2] = {0x00,0xff};
    serial.write((const char *)&start,2);
}
