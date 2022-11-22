#include "messenger.h"

Messenger::Messenger( QObject *parent) : QObject(parent)
{
    quint16 buffersize = 0;
    buf1 = new Buffer(buffersize);
    buf2 = new Buffer(buffersize);

    vso = new VoiceSocket();
    vio = new VoiceIO();

    // Voice in
    connect(vio,  SIGNAL(readVoice(QByteArray)), buf1, SLOT(input(QByteArray)));
    connect(buf1, SIGNAL(output(QByteArray)),    vso,  SLOT(writeData(QByteArray)));

    // Voice out
    connect(vso,  SIGNAL(readData(QByteArray, QString)),  this, SLOT(on_compare_ip(QByteArray,QString)));
    connect(this,  SIGNAL(read_checked_data(QByteArray)),  buf2, SLOT(input(QByteArray)));
    connect(buf2, SIGNAL(output(QByteArray)), vio, SLOT(writeVoice(QByteArray)));
}

void Messenger::set_host(QString dst)
{
    dst_addr = dst;
    vso->set_dst_addr(QHostAddress(dst), 30011);
}

void Messenger::on_compare_ip(QByteArray data, QString m_ip)
{
    if(m_ip != dst_addr){
        return ;
    }
    emit read_checked_data(data);
}
