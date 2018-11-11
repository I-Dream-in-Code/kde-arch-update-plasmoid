#include "systemCalls.h"
#include <qt/QtCore/qlogging.h>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>
#include <qt/QtCore/QMetaObject>
#include <qt/QtCore/QStringList>
#include <QTime>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3




systemCalls::systemCalls(QObject *parent) : QObject(parent)
{
	this->worker = new Worker;
	worker->moveToThread(&this->workerThread);
	connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
	connect(this, &systemCalls::checkUpdatesSignal, worker, &Worker::checkUpdates);
	connect(this, &systemCalls::upgradeSystemSignal, worker, &Worker::upgradeSystem);

	workerThread.start();
}



systemCalls::~systemCalls()
{
	workerThread.quit();
	workerThread.wait();

}
//https://karanbalkar.com/2014/02/detect-internet-connection-using-qt-5-framework/
Q_INVOKABLE bool systemCalls::isConnectedToNetwork()
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



Q_INVOKABLE void systemCalls::checkUpdates(bool namesOnly, bool aur)
{
	if(worker->upgradeProcessRunning)
		return;
	QTime deliTime;
// 	if (!systemCalls::isConnectedToNetwork())
// 	{
//
// 		qDebug() << "org.kde.archUpdate: still not connected returning 'No Internet Connection' to plasmoid";
// 		worker->updates = QStringList();
// 		worker->updates << "No Internet Connection";
// 		return;
//
// 	}
	worker->mutex = true;
	emit systemCalls::checkUpdatesSignal(namesOnly, aur);

}


Q_INVOKABLE void systemCalls::upgradeSystem(bool konsoleFlag, bool aur, bool noconfirm, bool yakuake, bool orphan, bool snapRefreshFlag)
{
	if(worker->upgradeProcessRunning)
		return;
	worker->mutex = true;
	worker->upgradeProcessRunning = true;
	emit systemCalls::upgradeSystemSignal(konsoleFlag, aur, noconfirm, yakuake, orphan,snapRefreshFlag);
}


Q_INVOKABLE QStringList systemCalls::readCheckUpdates()
{
	QTime deliTime;
	if(worker->upgradeProcessRunning)
		return QStringList();
	deliTime = QTime::currentTime().addMSecs(500);
	while(worker->mutex == true)
		QCoreApplication::processEvents();

	worker->mutex = false;
	qDebug() << "org.kde.archUpdate: Returning updates";
	return worker->updates;

}

void systemCalls::restartShell()
{
	this->killShellProcess = new QProcess();
	QStringList args3;
	args3 << "plasmashell";

	this->killShellProcess->start("killall", args3);

	this->killShellProcess->waitForFinished(-1);


	QProcess restartShell;
	restartShell.start("kstart5", args3);

	delete killShellProcess;
}


void systemCalls::pickNewIcon()
{
	QWidget fileDialog;
	QString fileName = "";

	fileName = QFileDialog::getOpenFileName(&fileDialog,
	                                        tr("Open Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp *.svg)"));

	qDebug() << "Selected " << fileName;

	setNewIcon(3, fileName);
}

bool systemCalls::setNewIcon(const int mode, const QString fileName)
{
		QStringList args;

		switch (mode) {
        case 0:
        {
            args << "cp" << "/usr/share/plasma/plasmoids/org.kde.archUpdate/contents/images/default.svg" << "/usr/share/plasma/plasmoids/org.kde.archUpdate/contents/images/chosen.svg";
			break;
        }
        case 1:
        {
            args << "cp" << "/usr/share/plasma/plasmoids/org.kde.archUpdate/contents/images/light.svg" << "/usr/share/plasma/plasmoids/org.kde.archUpdate/contents/images/chosen.svg";
			break;
        }
        case 2:
        {
            args << "cp" << "/usr/share/plasma/plasmoids/org.kde.archUpdate/contents/images/dark.svg" << "/usr/share/plasma/plasmoids/org.kde.archUpdate/contents/images/chosen.svg";
			break;
        }
		case 3:
        {
			if (!fileName.isEmpty())
            	args << "cp" << fileName << "/usr/share/plasma/plasmoids/org.kde.archUpdate/contents/images/chosen.svg";
			break;
        }
        }
		if (args.isEmpty())
			return false;
		this->CopyFileProcess = new QProcess();
		this->CopyFileProcess->start("pkexec", args);
		this->CopyFileProcess->waitForFinished(-1);
		const int exitCode = this->CopyFileProcess->exitCode();
		delete this->CopyFileProcess;
		if (exitCode == 0)
			return true;

		return false;
}


