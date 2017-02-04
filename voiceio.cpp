#include "voiceio.h"

#include <QAudio>
#include <QAudioInput>
#include <QAudioOutput>

#include <QDebug>

VoiceIO::VoiceIO(QObject *parent) : QObject(parent)
{    
    QAudioFormat format;
    format.setSampleRate(8000); // 22050
    format.setSampleSize(8);    // 16
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
        qDebug() << format.sampleRate();
        qDebug() << format.sampleSize();
    }

    QAudioInput *InputAudio = new QAudioInput(format, this);

    QAudioOutput *OutputAudio = new QAudioOutput(format, this);

    devInput  = InputAudio->start();
    qDebug() << "Started " << info.deviceName();

    //OutputAudio->start(devInput);
    devOutput = OutputAudio->start();
    QAudioDeviceInfo infoOutput(QAudioDeviceInfo::defaultOutputDevice());
    qDebug() << "Started " << infoOutput.deviceName();

    connect(devInput, SIGNAL(readyRead()), this, SLOT(devInput_readyRead()));
}

void VoiceIO::writeVoice(QByteArray data)
{
    devOutput->write(data);
}

void VoiceIO::devInput_readyRead()
{
    QByteArray data = devInput->readAll();
    emit readVoice(data);
}
