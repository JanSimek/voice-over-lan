#include "voicesocket.h"
#define SampleRate 44100
#define ChannelCnt 1
#define SampleSize 32
#define BUFF_SIZE           4000

VoiceSocket::VoiceSocket(QObject *parent) :
    QObject(parent)
{
    //isEnabled = false;
    connect(&udp, SIGNAL(readyRead()), this, SLOT(udp_readyRead()));
    pitch_val = 0;
}

void VoiceSocket::set_dst_addr(QHostAddress ip, quint16 port)
{
    peer_ip = ip;
    peer_port = port;
}

void VoiceSocket::startListen(quint16 port)
{
    udp.bind(QHostAddress::Any, port);
}

void VoiceSocket::writeData(QByteArray data)
{
    QByteArray pitch_buffer;
    process(data, pitch_buffer);
    QByteArray data2 = qCompress(pitch_buffer, 9);
    udp.writeDatagram(data2, peer_ip, peer_port);
}

void VoiceSocket::udp_readyRead()
{
    int size = udp.pendingDatagramSize();
    QHostAddress host;
    quint16 port;
    QByteArray data;
    data.resize(size);
    udp.readDatagram(data.data(), size, &host, &port);
    QString b_host = host.toString();
    QString R_host = b_host.right(b_host.length() - b_host.indexOf(":",3)-1);
    QByteArray data2 = qUncompress(data);
    emit readData(data2, R_host);
}

void VoiceSocket::process(QByteArray in, QByteArray& out)
{
#ifdef soundtouch2
    SoundTouch  pSoundTouch;
    pSoundTouch.setSampleRate(SampleRate);
    pSoundTouch.setChannels(ChannelCnt);
    pSoundTouch.setPitchSemiTones(pitch_val);

    int nSamples;
    int nChannels;
    int buffSizeSamples;

    pSoundTouch.putSamples((SAMPLETYPE*)(in.data() ), in.count() / 4);
    SAMPLETYPE sampleBuffer[BUFF_SIZE];
    pSoundTouch.flush();
    do
    {
        nSamples = pSoundTouch.receiveSamples(sampleBuffer, BUFF_SIZE);
        out.append((char*)sampleBuffer, nSamples*4);
    } while (nSamples != 0);
#endif
}
