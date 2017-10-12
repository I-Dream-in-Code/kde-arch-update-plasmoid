#include "systemCalls.h"
#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include <QtConcurrent/QtConcurrentRun>

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3

systemCalls::systemCalls(QObject* parent) : QObject(parent) { }


//https://karanbalkar.com/2014/02/detect-internet-connection-using-qt-5-framework/
bool systemCalls::iscConnectedToNetwork()
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

QString systemCalls::getAURHelper()
{
	//searches /usr/bin for AUR helpers and returns the first entry without trailing \n
	getAURHelperProcess = new QProcess(this);
	QStringList arguments;
	arguments << "/usr/bin/" << "|" << "egrep" << "'(apacman|aura|auracle|aurel|aurget|aurquery|aurutils|bauerbill|burgaur|cower|cylon|owlman|pacaur|pacget|packer|pamac|pbget|PKGBUILDer|prm|repoctl|spinach|trizen|wrapaur|yaah|yaourt|yay)'" << "|" << " head" << "-n1" << "|" << "tr" << "-d" << "'\n'";
	getAURHelperProcess->start("usr/bin/ls", arguments);
	return getAURHelperProcess->readAllStandardOutput();
}

QStringList systemCalls::checkUpdates(bool namesOnly, bool aur)
{
	QString aurPackages;
	QStringList aurResultsVector;

	if (!iscConnectedToNetwork())
		return QStringList() << "No Internet Connection";

	qDebug() << "clicked" << endl;
	//starts checkupdates as new qProcess
	checkUpdatesProcess = new QProcess(this);

	if (aur)
	{
		qDebug() << "=========== AUR ===========";
		QProcess *checkUpdatesAURProcess = new QProcess(this);
		checkUpdatesAURProcess->start("/usr/bin/checkupdates-aur");

		if (checkUpdatesAURProcess->waitForReadyRead(-1))
		{
			checkUpdatesAURProcess->waitForFinished(-1);
			aurPackages = checkUpdatesAURProcess->readAllStandardOutput();
// 			qDebug() <<aurPackages;
		}

		aurResultsVector = aurPackages.split(((QRegExp) "\n"));
		aurResultsVector.removeAt(aurResultsVector.length() - 1);
		qDebug() << aurResultsVector;
	}

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
			//split into vector by \n
			QStringList resultsVector = results.split(((QRegExp) "\n"));
			//remove trailing ""
			resultsVector.removeAt(resultsVector.length() - 1);

			//add aur packages
			for (int i = 0; i < aurResultsVector.length(); i++)
			{
				resultsVector.push_back(aurResultsVector[i]);
			}

			//sort vector so aur packages aren't at the bottom
			qSort(resultsVector.begin(), resultsVector.end());
			qDebug() << "=========CHECK UPDATES SPLIT============" << resultsVector;
			//if namesOnly is supplied as argument, return only package names without version upgrade information
			QStringList namesOnlyResults;

			if (namesOnly)
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
		return QStringList() << "cannot start checkupdates";
	}
}

void systemCalls::showProgressInqDebug()
{
	qDebug() << systemUpdateProcess->readAllStandardOutput();
}


int systemCalls::upgradeSystem(bool konsoleFlag, bool aur)
{
	if (!iscConnectedToNetwork())
		return NO_INTERNET;

	systemUpdateProcess = new QProcess(this);
	connect(systemUpdateProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgressInqDebug()));
	qDebug() << "KONSOLE FLAG" << konsoleFlag;

	if (konsoleFlag && aur)
	{
		QStringList arguments;
		arguments << "--hold" << "-e" << "sudo" << getAURHelper() << "-Syyu" << "--noconfirm" << "--force";
		systemUpdateProcess->start("/usr/bin/konsole", arguments);
	}

	else if (aur)
	{
		QStringList arguments;
		arguments << getAURHelper() << "-Syyu" << "--noconfirm" << "--force";
		systemUpdateProcess->start("pkexec", arguments);
	}

	else
	{
		QStringList arguments;
		arguments << "/usr/bin/pacman" << "-Syyu" << "--noconfirm" << "--force";
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

			if (log.indexOf("file exists") != -1)
			{
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



void systemCalls::upgradeConcurrent(bool konsoleFlag, bool aur)
{
	QFuture<void> future = QtConcurrent::run(&systemCalls::upgradeSystem, konsoleFlag, aur);
}

QStringList systemCalls::checkUpdatesConcurrent(bool namesOnly, bool aur)
{
	QFuture<QStringList> future = QtConcurrent::run(&systemCalls::checkUpdates, namesOnly, aur);
	return future.result();
}
