#ifndef WORKER_H
#define WORKER_H

class Worker : public QObject
{
		Q_OBJECT

	public:

		QString getAURHelper();
		QStringList getAURHelperCommands(QString AURHelper);
		QStringList updates;
	signals:
		void readCheckUpdatesSignal(QStringList &results);
		void promptPassword();
	public slots:
		void checkUpdates(bool namesOnly, bool aur);
		void upgradeSystem(bool konsoleFlag, bool aur);

};
#endif
