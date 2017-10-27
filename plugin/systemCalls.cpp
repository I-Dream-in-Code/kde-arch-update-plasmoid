#include "systemCalls.h"
#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3

systemCalls::systemCalls(QObject* parent) : QObject(parent) { }


//https://karanbalkar.com/2014/02/detect-internet-connection-using-qt-5-framework/
bool systemCalls::isConnectedToNetwork()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;
    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if (iface.flags().testFlag(QNetworkInterface::IsUp)
                && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            for (int j = 0; j < iface.addressEntries().count(); j++)
            {
                // got an interface which is up, and has an ip address
                if (result == false)
                    result = true;
            }
        }
    }
    return result;
}


QStringList systemCalls::checkUpdates(QString arguments)
{
    if (!isConnectedToNetwork())
        return QStringList()<<"No Internet Connection";
    qDebug() << "clicked" << endl;
    //starts checkupdates as new qProcess
    checkUpdatesProcess = new QProcess(this);
    checkUpdatesProcess->start("/usr/bin/checkupdates");
    //waits until started 3000 msec timeout
    if (checkUpdatesProcess->waitForStarted(3000))
    {
        qDebug() << "check updates started" << endl;
        //DEBUGGING WILL REMOVE
        if (checkUpdatesProcess->waitForReadyRead(-1))
        {
			checkUpdatesProcess->waitForFinished(-1);
            QString results = checkUpdatesProcess->readAllStandardOutput();
            qDebug() << "================CHECKUPDATES CALL===================" << results;
            QStringList resultsVector = results.split(((QRegExp) "\n"));
			resultsVector.removeAt(resultsVector.length()-1);
            qDebug() << "=========CHECK UPDATES SPLIT============" << resultsVector;
            //if namesOnly is supplied as argument, return only package names without version upgrade information
            QStringList namesOnlyResults;
            size = resultsVector.length();
            if (arguments == "namesOnly")
            {
                QVector<QStringList> tmp;
                for (int i = 0; i < resultsVector.length(); i++)
                    tmp.push_back(resultsVector[i].split((QRegExp) " "));
                for (int i = 0; i < tmp.length(); i++)
                    namesOnlyResults.push_back(tmp[i][0]);
                qDebug() << "==========NAMES ONLY================" << namesOnlyResults;
                return namesOnlyResults;
            }
            else
                return resultsVector;
        }
        else
        {
            qDebug() << "Cannot read from /usr/bin/checkupdates";
            qDebug() << checkUpdatesProcess->error() << endl << checkUpdatesProcess->errorString();
            return QStringList();
        }
    }
    else
    {
        qDebug() << "Cannot start /usr/bin/checkupdates" << endl;
        qDebug() << checkUpdatesProcess->error() << endl << checkUpdatesProcess->errorString();
        return QStringList() <<"cannot start checkupdates";
    }
}

void systemCalls::showProgressInqDebug()
{
    qDebug() << systemUpdateProcess->readAllStandardOutput();
}


int systemCalls::upgradeSystem(bool konsoleFlag)
{
    if (!isConnectedToNetwork())
        return NO_INTERNET;
    systemUpdateProcess = new QProcess(this);
    connect(systemUpdateProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgressInqDebug()));
    QStringList arguments;
	qDebug()<<"KONSOLE FLAG"<< konsoleFlag;
	//if user selects show in konsole in settings display in konsole
	if(konsoleFlag){
		arguments << "--hold" << "-e" << "sudo"<< "pacman" << "-Syu" << "--noconfirm" << "--force";
		systemUpdateProcess->start("/usr/bin/konsole",arguments);
	}
	else
		//if user does not select show in konsole run pexec
	{ 
		arguments << "/usr/bin/pacman" << "-Syu" << "--noconfirm" << "--force";
		systemUpdateProcess->start("pkexec", arguments);
	}
		// calls Qprocess for "pexec pacman -Syu --noconfirm --force"
   
    //wait to start 3000 msec timeout
    if (systemUpdateProcess->waitForStarted(3000))
    {
        qDebug() << "STARTED";
        //wait for finish no timeout
        if (systemUpdateProcess->waitForFinished(-1))
        {	
			QString log = systemUpdateProcess->readAllStandardOutput();
			if(log.indexOf("file exists")!=-1){
				return CONFLICTS;
			}
            qDebug() << "=====UPDATE COMPLETE";
            return SUCCESS;
        }
        else
        {
            qDebug() << "cannot finish";
            return systemUpdateProcess->exitCode();
        }
    }
    else
    {
        qFatal("cannot start");
        qDebug() << systemUpdateProcess->error() << endl << systemUpdateProcess->errorString();
		return CANNOT_START;
    }
}

void systemCalls::upgradeConcurrent(bool konsoleFlag){
	QtConcurrent::run(this,&systemCalls::upgradeSystem,konsoleFlag);
}
