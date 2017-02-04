#include "messenger.h"

Messenger::Messenger(QString address, QObject *parent) : QObject(parent)
{
    quint16 buffersize = 0;
    buf1 = new Buffer(buffersize);
    buf2 = new Buffer(buffersize);

    vso = new VoiceSocket();
    vio = new VoiceIO();

    if(address != "")
    {
        vso->connectToHost(QHostAddress(address), 30011); // QHostAddress::LocalHost
        qDebug() << "Connecting to " << address;
    }
    else
    {
        qDebug() << "No peer address specified. Will only play sound from others";
    }

    vso->setEnabled(true);
    vso->startListen();

    // Voice in
    connect(vio,  SIGNAL(readVoice(QByteArray)), buf1, SLOT(input(QByteArray)));
    connect(buf1, SIGNAL(output(QByteArray)),    vso,  SLOT(writeData(QByteArray)));

    // Voice out
    connect(vso,  SIGNAL(readData(QByteArray)),  buf2, SLOT(input(QByteArray)));
    connect(buf2, SIGNAL(output(QByteArray)), vio, SLOT(writeVoice(QByteArray)));
}
