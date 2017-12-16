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

#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3




systemCalls::systemCalls(QObject* parent) : QObject(parent)
{
	this->worker = new Worker;
	worker->moveToThread(&this->workerThread);
	connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
	connect(this, &systemCalls::checkUpdatesSignal, worker, &Worker::checkUpdates);
	connect(this, &systemCalls::upgradeSystemSignal, worker, &Worker::getAURHelper);
	this->passwordWorker = new PasswordWorker;
	passwordWorker->moveToThread(&this->passwordWorkerThread);
	connect(&passwordWorkerThread, &QThread::finished, passwordWorker, &QObject::deleteLater);
	connect(this, SIGNAL(promptPassword()), passwordWorker, SLOT(promptPassword()));
	
	
	passwordWorkerThread.start();
	
	workerThread.start();
}



systemCalls::~systemCalls()
{
	workerThread.quit();
	workerThread.wait();
	passwordWorkerThread.quit();
	passwordWorkerThread.wait();
}
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



Q_INVOKABLE QStringList systemCalls::checkUpdates(bool namesOnly, bool aur)
{
	if (!systemCalls::isConnectedToNetwork())
		return QStringList() << "No Internet Connection";

	QStringList r = emit systemCalls::checkUpdatesSignal(namesOnly, aur);
	return r;

}
Q_INVOKABLE int systemCalls::upgradeSystem(bool konsoleFlag, bool aur)
{
	int r = emit systemCalls::upgradeSystemSignal(konsoleFlag, aur);
	return r;

}



QStringList systemCalls::readCheckUpdates()
{
	return worker->updates;
}
