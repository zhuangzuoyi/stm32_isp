#ifndef SEND_THREAD_H
#define SEND_THREAD_H

#include <QThread>
#include "QSerialPort"



class send_thread : public QThread
{
    Q_OBJECT
public:
    send_thread();
    void set_data(QByteArray dat);
    void set_serial(QSerialPort *com);
    void set_delay(unsigned long time);
signals:

public slots:

private:
    QByteArray cmd;
    QSerialPort *serial;
    unsigned long Delay;
protected:
    void run();
};

#endif // SEND_THREAD_H
