#ifndef PASSWORD_WORKER_H
#define PASSWORD_WORKER_H
#include <QObject>
class PasswordWorker : public QObject
{
		Q_OBJECT

	public:
	
	 signals:
		
		void workerUpgradeSystemSignal(bool konsoleFlag, bool aur, QString password);
	 public slots:
		void promptPassword(bool konsoleFlag, bool aur);
};

#endif
