#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QsLog.h>
#include <QsLogDest.h>

using namespace QsLogging;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Setup logger, logs will be saved to app_dir/logs/log.txt files
    Logger& logger = Logger::instance();
#ifdef QT_DEBUG
    logger.setLoggingLevel(QsLogging::TraceLevel);
#else
    logger.setLoggingLevel(QsLogging::InfoLevel);
#endif
    QDir logsDir(a.applicationDirPath() + QDir::separator() + "logs/");
    if(!logsDir.exists()) {
        logsDir.mkdir(logsDir.path());
    };
    const QString sLogPath(logsDir.filePath("log.txt"));

    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
       sLogPath, EnableLogRotation, MaxSizeBytes(2000000), MaxOldLogCount(2)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    MainWindow w;
    w.show();

    return a.exec();
}
