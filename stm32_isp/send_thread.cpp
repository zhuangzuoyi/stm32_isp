#include "send_thread.h"
#include "QDebug"

send_thread::send_thread()
{

}


void send_thread::set_data(QByteArray dat)
{
    cmd = dat;
}


void send_thread::set_delay(unsigned long time)
{
    Delay = time;
}


void send_thread::run()
{
    if(cmd.isEmpty())
          return;
    qDebug()<<"The delay is:"<<Delay;
    for(int i=0;i<cmd.length();i++)
    {
        usleep(Delay);
        emit timerout();
    }

}
