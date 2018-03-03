#ifndef WORKER_H
#define WORKER_H

class Worker : public QObject
{
		Q_OBJECT
        private:
                QString prepareYakuake();

	public:
		static QMutex mutex;
		static bool upgradeProcessRunning;
		QString getAURHelper();
		QStringList getAURHelperCommands(QString AURHelper);
		QStringList updates;
		static bool wait;
	signals:
		void readCheckUpdatesSignal(QStringList &results);
	public slots:

		void checkUpdates(bool namesOnly, bool aur);
		void upgradeSystem(bool konsoleFlag, bool aur, bool noconfirm, bool yakuake);

};
#endif
