#include "voiceio.h"
#define SampleRate 44100
#define ChannelCnt 1
#define SampleSize 32
#define BUFF_SIZE           4000

VoiceIO::VoiceIO(QObject *parent) : QObject(parent)
{    

    formatAudio.setSampleRate(SampleRate);
    formatAudio.setChannelCount(ChannelCnt);
    formatAudio.setSampleSize(SampleSize);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::Float);

    m_audioInput = new QAudioInput(QAudioDeviceInfo::defaultInputDevice(), formatAudio, this);
    m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), formatAudio, this);

    devInput  = m_audioInput->start();
    m_buffer.clear();
    m_audioOutputIODevice.close();
    m_audioOutputIODevice.setBuffer(&m_buffer);
    m_audioOutputIODevice.open(QIODevice::ReadOnly);
    m_audioOutput->start(&m_audioOutputIODevice);

    connect(devInput, SIGNAL(readyRead()), this, SLOT(devInput_readyRead()));
}

void VoiceIO::writeVoice(QByteArray data)
{
    m_buffer.append(data);
}

void VoiceIO::devInput_readyRead()
{
    QByteArray data = devInput->readAll();
    emit readVoice(data);
}
