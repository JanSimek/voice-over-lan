#ifndef MESSENGER_H
#define MESSENGER_H

#include <QAudio>
#include <QAudioInput>
#include <QAudioOutput>
#include <QDebug>
#include "buffer.h"
#include "voiceio.h"
#include "voicesocket.h"
#include <QSqlQuery>
#include <QSystemTrayIcon>
#include <QSqlDatabase>
#include <QSystemTrayIcon>

class Messenger : public QObject
{
    Q_OBJECT

public:

    explicit Messenger( QObject *parent = 0);
    QString dst_addr;
    void set_host(QString dst);
     VoiceSocket* vso;

signals:
     void read_checked_data(QByteArray);

public slots:
     void on_compare_ip(QByteArray, QString);

private:
    VoiceIO* vio;
    Buffer* buf1;
    Buffer* buf2;
};

#endif // MESSENGER_H
