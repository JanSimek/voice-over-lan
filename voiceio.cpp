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

    QAudioDeviceInfo inputInfo = QAudioDeviceInfo::defaultInputDevice();
    if (!inputInfo.isFormatSupported(format))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = inputInfo.nearestFormat(format);
    }
    qDebug() << "In device  : " << inputInfo.deviceName();
    qDebug() << "Sample rate: " << format.sampleRate();
    qDebug() << "Sample size: " << format.sampleSize();
    qDebug() << "Channel cnt: " << format.channelCount();
    qDebug() << "Codec      : " << format.codec();

    QAudioInput *InputAudio = new QAudioInput(format, this);
    devInput = InputAudio->start();

    QAudioDeviceInfo outputInfo = QAudioDeviceInfo::defaultOutputDevice();
    if (!outputInfo.isFormatSupported(format))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = outputInfo.nearestFormat(format);
    }
    qDebug() << "Out device : " << outputInfo.deviceName();
    qDebug() << "Sample rate: " << format.sampleRate();
    qDebug() << "Sample size: " << format.sampleSize();
    qDebug() << "Channel cnt: " << format.channelCount();
    qDebug() << "Codec      : " << format.codec();

    QAudioOutput *OutputAudio = new QAudioOutput(format, this);
    devOutput = OutputAudio->start();

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
