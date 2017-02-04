#ifndef MESSENGER_H
#define MESSENGER_H

#include <QUdpSocket>

#include <QAudio>
#include <QAudioInput>
#include <QAudioOutput>

#include <QDebug>

#include "buffer.h"
#include "voiceio.h"
#include "voicesocket.h"

class Messenger : public QObject
{
    Q_OBJECT
public:
    explicit Messenger(QString address, QObject *parent = 0);

signals:

public slots:

private:
    QUdpSocket _udp;

    // Voice
    VoiceIO* vio;
    VoiceSocket* vso;
    Buffer* buf1;
    Buffer* buf2;
};

#endif // MESSENGER_H
