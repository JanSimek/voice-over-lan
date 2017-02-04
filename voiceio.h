#ifndef VOICEIO_H
#define VOICEIO_H

#include <QIODevice>

class VoiceIO : public QObject
{
    Q_OBJECT
public:
    explicit VoiceIO(QObject *parent = 0);

signals:
    void readVoice(QByteArray data);

public slots:
    void writeVoice(QByteArray data);

private slots:
    void devInput_readyRead();

private:
    QIODevice* devInput;
    QIODevice* devOutput;

};

#endif // VOICEIO_H
