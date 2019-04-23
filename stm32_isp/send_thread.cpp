#include "send_thread.h"

send_thread::send_thread()
{

}


void send_thread::set_data(QByteArray dat)
{
    cmd = dat;
}


void send_thread::set_serial(QSerialPort *com)
{
    serial = com;
}

void send_thread::set_delay(unsigned long time)
{
    Delay = time;
}


void send_thread::run()
{
    if(cmd.isEmpty())
          return;
    if(serial->isOpen())
    {
        for(int i=0;i<cmd.length();i++)
        {
           // unsigned char sending = (unsigned char)cmd.at(i);
            char sending = cmd.at(1);
            serial->write(&sending,1);
            //serial->moveToThread()
            usleep(Delay);
        }
    }
}
