#include "mainwindow.h"
#include <QApplication>

#include "messenger.h"

#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("voice-over-lan");
    a.setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Voice over LAN");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("address", QApplication::translate("main", "Address of the counterpart"));

    parser.process(a);

    QString address("");
    if(!parser.positionalArguments().isEmpty())
    {
        address = parser.positionalArguments().at(0);
    }

    Messenger msgr(address);

    //MainWindow w;
    //w.show();

    return a.exec();
}
