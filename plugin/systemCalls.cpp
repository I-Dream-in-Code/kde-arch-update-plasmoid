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
#include "passwordWorker.h"

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
// 	connect(this, &systemCalls::upgradeSystemSignal, passwordWorker, SLOT(promptPassword));
	this->passwordWorker = new PasswordWorker;
	passwordWorker->moveToThread(&this->passwordWorkerThread);
	connect(&passwordWorkerThread, &QThread::finished, passwordWorker, &QObject::deleteLater);
// 	connect(this->worker, SIGNAL(promptPasswordSi()), passwordWorker, SLOT(promptPassword()));
// 	connect(this->passwordWorker, SIGNAL(setPasswordSignal), this->worker, SLOT(setPassword));
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



Q_INVOKABLE void systemCalls::checkUpdates(bool namesOnly, bool aur)
{
	if (!systemCalls::isConnectedToNetwork())
	{
		worker->updates = QStringList();
		worker->updates << "No Internet Connection";
		return;
	}

	emit systemCalls::checkUpdatesSignal(namesOnly, aur);
}
Q_INVOKABLE void systemCalls::upgradeSystem(bool konsoleFlag, bool aur)
{
	if (!systemCalls::isConnectedToNetwork())
	{
		worker->updates = QStringList();
		worker->updates << "No Internet Connection";
		return;
	}

	emit systemCalls::promptPasswordSignal(konsoleFlag, aur);
}



Q_INVOKABLE QStringList systemCalls::readCheckUpdates()
{
	return worker->updates;
}
