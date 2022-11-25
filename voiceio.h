#ifndef VOICEIO_H
#define VOICEIO_H

#include <QIODevice>
#include <QAudio>
#include <QAudioInput>
#include <QAudioOutput>
#include <QDebug>
#include <QBuffer>

class VoiceIO : public QObject
{
    Q_OBJECT
public:
    explicit VoiceIO(QObject *parent = 0);

    QAudioFormat        formatAudio;
    QAudioInput*        m_audioInput;
    QAudioOutput*       m_audioOutput;
    QBuffer             m_audioOutputIODevice;
    QByteArray          m_buffer;

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
