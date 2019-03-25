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
            port = menuBar()->addMenu(menu_temp);
            started = true;
        }
        port->addAction(action);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
