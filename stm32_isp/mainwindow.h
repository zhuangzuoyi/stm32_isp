#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSerialPort"
#include "QTimer"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
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
    int stm32_sync(void);
    int stm32_get(void);
    void stm32_get_version(void);
    void stm32_get_id(void);
    void timer_update(void);
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QMenu *port;
    QMenu *baudrate_menu;

    QTimer *timer;
    QString current_port;
    int current_baudrate;

    QSerialPort serial;

    void serial_port_init(void);
    void isp_send_cmd_task(void);
    QByteArray send_buf;
    int send_index;
};

#endif // MAINWINDOW_H
