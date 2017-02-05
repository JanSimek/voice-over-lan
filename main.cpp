#include <QCoreApplication>
#include <QCommandLineParser>

#include "messenger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("voice-over-lan");
    a.setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Voice over LAN");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("address", QCoreApplication::translate("main", "Address of the counterpart"));

    parser.process(a);

    QString address("");
    if(!parser.positionalArguments().isEmpty())
    {
        address = parser.positionalArguments().at(0);
    }

    Messenger msgr(address);

    return a.exec();
}
