#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSerialPort"

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

private:
    Ui::MainWindow *ui;
    QMenu *port;
    QMenu *baudrate_menu;

    QString current_port;
    int current_baudrate;

    QSerialPort serial;

    void serial_port_init(void);
};

#endif // MAINWINDOW_H
