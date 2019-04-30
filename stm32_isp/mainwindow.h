#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSerialPort"
#include "QTimer"
#include "send_thread.h"


namespace Ui {
class MainWindow;
}


#define ACK  0x79
#define NACK 0x1f
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:


    enum Isp_cmd  {
        Isp_Sync = 1,
        Isp_get = 2,
        Isp_get_version = 3,
        Isp_get_id = 4,
        Isp_Read_men = 5,
        Isp_go =6,
        Isp_write_men =7,
        Isp_erase = 8
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void serial_open(void);
private slots:
    void port_menu_click(QAction* act);
    void baud_menu_click(QAction* act);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void serial_read_dat(void);
    void on_pushButton_3_clicked();
    void timer_update(void);
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void isp_send_task(void);
    void on_clear_clicked();

private:
    Ui::MainWindow *ui;
    QMenu *port;
    QMenu *baudrate_menu;

    QTimer *timer;
    QString current_port;
    int current_baudrate;

    QSerialPort serial;
    QByteArray send_buf;
    QByteArray read_buf;

    QByteArray support_cmd;
    int send_index;

    int current_cmd;
    int current_cmd_status;

    int stm32_sync(void);
    int stm32_get(void);
    void stm32_get_version(void);
    void stm32_get_id(void);

    void serial_port_init(void);
    void isp_send_cmd_task(void);
    void stm32_send_cmd(QByteArray cmd_data,int cmd);
    void stm32_isp_read_data(QByteArray  dat);
    void show_msg(QString msg);

    send_thread *Sending;
    unsigned long sending_delay;
};

#endif // MAINWINDOW_H
