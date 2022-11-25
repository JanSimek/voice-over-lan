#ifndef VOICESOCKET_H
#define VOICESOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <soundtouch/SoundTouch.h>

using namespace soundtouch;
using namespace std;

class VoiceSocket : public QObject
{
    Q_OBJECT
public:
    explicit    VoiceSocket(QObject *parent = 0);
    void        set_dst_addr(QHostAddress ip, quint16 port = 30011);
    void        startListen(quint16 port = 30011);
    void        process(QByteArray in, QByteArray& out);
    int         pitch_val;

signals:
    void readData(QByteArray data, QString);

public slots:
    void writeData(QByteArray data);

private slots:
    void udp_readyRead();

private:
    QUdpSocket udp;
    QHostAddress peer_ip;
    quint16 peer_port;
};

#endif // VOICESOCKET_H
