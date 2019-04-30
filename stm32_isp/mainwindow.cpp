#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSerialPort"
#include "QDebug"
#include "QSerialPortInfo"
#include "QAction"
#include "send_thread.h"


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
//    Baudrate.append("460800");
//    Baudrate.append("256000");
//    Baudrate.append("230400");
//    Baudrate.append("128000");
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
    }
   connect(baudrate_menu,SIGNAL(triggered(QAction*)),this,SLOT(baud_menu_click(QAction*)));

   current_baudrate = Baudrate.at(0).toInt();
   serial_port_init();

   timer = new QTimer(this);
   connect(timer,SIGNAL(timeout()),this,SLOT(timer_update()));
   connect(&serial, &QSerialPort::readyRead, this,&MainWindow::serial_read_dat);

   sending_delay = 10;
   Sending = new send_thread();
   connect(Sending, SIGNAL(timerout()), this, SLOT(isp_send_task()));
   read_buf.clear();
}

void MainWindow::timer_update(void)
{
//    isp_send_cmd_task();
//    qDebug()<<"Time update";
}

void MainWindow::stm32_isp_read_data(QByteArray  dat)
{
    uint8_t data;
    read_buf.append(dat);
    if(read_buf.isEmpty())
        return;
    switch(current_cmd)
    {
    case Isp_Sync:
        data =(uint8_t) read_buf.back();
        if(data == ACK || data == NACK)
        {
            current_cmd_status = 2;
            qDebug()<<"sync success";
            show_msg("sync success");
            read_buf.clear();
        }
        break;

    case Isp_get:
        if(read_buf.length()>1 && read_buf.endsWith(0x79))
        {
            int first_ack = read_buf.indexOf(0x79);
            int second_ack = read_buf.indexOf(0x79,first_ack+1);
            qDebug()<<"first ack:"<<first_ack;
            qDebug()<<"second ack:"<<second_ack<<","<<QString::number((first_ack + read_buf[first_ack+1]+3),16)<<";";


            if((first_ack + read_buf[first_ack+1]+3) == second_ack )
            {
                QString cmd_list;
                QString temp;

                QString bootload_version = QString::number(read_buf[first_ack+2],16);
                cmd_list.append("    Version:");
                cmd_list.append(bootload_version.at(0));
                cmd_list.append(".");
                cmd_list.append(bootload_version.at(1));
                cmd_list.append("\r\n    cmd list:");

                int len = read_buf[first_ack+1];
                for(int i=0;i<len;i++)
                {
                    temp = QString::number(read_buf[first_ack+3+i] & 0x000000ff,16);
                    if(temp.length() ==1)
                        cmd_list.append(" 0"+temp);
                    else {
                        cmd_list.append(" "+temp);
                    }
                }

                show_msg("get cmd:\r\n" + cmd_list);
                qDebug()<<read_buf;
                read_buf.clear();
            }
        }else
        {
          char  da = read_buf.back();
          if(da == NACK)
          {

          }
        }
        break;
    case Isp_get_version:
        if(read_buf.length()>1 && read_buf.endsWith(0x79))
        {
            int first_ack = read_buf.indexOf(0x79);
            int second_ack = read_buf.indexOf(0x79,first_ack+1);

            if((second_ack - first_ack ) == 4)
            {
                QString version;
                QString temp = QString::number(read_buf[first_ack+1],16);
                if(temp.length()==1)
                    version.append(" 0"+temp);
                else
                    version.append(" "+temp);

                temp = QString::number(read_buf[first_ack+2],16);
                if(temp.length()==1)
                    version.append(" 0"+temp);
                else
                    version.append(" "+temp);

                temp = QString::number(read_buf[first_ack+3],16);
                if(temp.length()==1)
                    version.append(" 0"+temp);
                else
                    version.append(" "+temp);

                show_msg("get version:"+version);
                qDebug()<<read_buf;
                read_buf.clear();
            }
        }else
        {
          char  da = read_buf.back();
          if(da == NACK)
          {

          }
        }
        break;
    case Isp_get_id:
        if(read_buf.length()>1 && read_buf.endsWith(0x79))
        {
            int first_ack = read_buf.indexOf(0x79);
            int second_ack = read_buf.indexOf(0x79,first_ack+1);

            if((second_ack - first_ack ) == 4)
            {
                QString id;
                QString temp = QString::number(read_buf[first_ack+1],16);
                if(temp.length()==1)
                    id.append(" 0"+temp);
                else
                    id.append(" "+temp);

                temp = QString::number(read_buf[first_ack+2],16);
                if(temp.length()==1)
                    id.append(" 0"+temp);
                else
                    id.append(" "+temp);

                temp = QString::number(read_buf[first_ack+3],16);
                if(temp.length()==1)
                    id.append(" 0"+temp);
                else
                    id.append(" "+temp);

                show_msg("get id:"+id);

                qDebug()<<read_buf;
                read_buf.clear();
            }
        }else
        {
          char  da = read_buf.back();
          if(da == NACK)
          {

          }
        }
        break;
    case Isp_Read_men:
        break;
    case Isp_go:
        break;
    case Isp_write_men:
        break;
    case Isp_erase:
        break;
    default:
        break;
    }
}
void MainWindow::serial_read_dat(void)
{
    stm32_isp_read_data(serial.readAll());
}


void MainWindow::port_menu_click(QAction* act)
{
    for(QAction *action:port->actions())
    {
        action->setIcon(QIcon::fromTheme("document-save", QIcon(":/images/none.png")));
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
        Sending->set_delay(1);
    }
}

void MainWindow::stm32_send_cmd(QByteArray cmd_data,int cmd)
{
    serial_open();
    send_buf.resize(cmd_data.length());
    for(int i=0;i<cmd_data.length();i++)
    {
        send_buf[i] = cmd_data[i];
    }
    send_index = 0;
    current_cmd_status = 1;
    current_cmd = cmd;

    Sending->set_data(send_buf);
    Sending->start();
//    timer->start(1);
}

int MainWindow::stm32_sync(void)
{
    QByteArray readed;
    serial_open();
    readed.resize(2);
    readed[0] = 0x7f;
    readed[1] = 0x7f;
    stm32_send_cmd(readed,Isp_Sync);
    return -1;
}


void MainWindow::isp_send_cmd_task(void)
{
//    uint8_t cmd = send_buf[send_index];
//    serial.write((char *)&cmd,1);
//    send_index ++ ;
//    if(send_index == send_buf.length())
//    {
//           timer->stop();
//           send_index = 0;
//    }
}

void MainWindow::isp_send_task(void)
{
    qDebug()<<"Sending";
    uint8_t cmd = send_buf[send_index];
    serial.write((char *)&cmd,1);
    send_index ++ ;
    if(send_index == send_buf.length())
    {
           send_index = 0;
    }
}

int MainWindow::stm32_get(void)
{
    QByteArray readed;
    serial_open();
    readed.resize(2);
    readed[0] = 0x00;
    readed[1] = 0xff;
//    unsigned char cmd[2] = {0x00,0xff};
    stm32_send_cmd(readed,Isp_get);
//    serial.write((char *)&cmd,2);
    return -1;
}

void MainWindow::stm32_get_version(void)
{
    QByteArray readed;
    serial_open();
    readed.resize(2);
    readed[0] = 0x01;
    readed[1] = 0xfe;
    stm32_send_cmd(readed,Isp_get_version);

}

void MainWindow::stm32_get_id(void)
{
    QByteArray readed;
    serial_open();
    readed.resize(2);
    readed[0] = 0x02;
    readed[1] = 0xfd;
    stm32_send_cmd(readed,Isp_get_id);
}


void MainWindow::on_pushButton_clicked()
{
    stm32_sync();
}

void MainWindow::on_pushButton_2_clicked()
{
    serial.close();
}

void MainWindow::on_pushButton_3_clicked()
{
    stm32_get();
}

void MainWindow::on_pushButton_4_clicked()
{
    stm32_get_id();
}

void MainWindow::on_pushButton_5_clicked()
{
    stm32_get_version();
}


void MainWindow::show_msg(QString msg)
{
    ui->sysmsg->append(msg);
}

void MainWindow::on_pushButton_6_clicked()
{
    sending_delay += 1;
    Sending->set_delay(sending_delay);
}

void MainWindow::on_pushButton_7_clicked()
{
    QByteArray readed;
    serial_open();
    readed.resize(2);
    readed[0] = 0x11;
    readed[1] = 0xee;
    stm32_send_cmd(readed,Isp_Read_men);
}

void MainWindow::on_clear_clicked()
{
    this->ui->sysmsg->clear();
}
