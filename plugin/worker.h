#ifndef WORKER_H
#define WORKER_H

class Worker : public QObject
{
    Q_OBJECT

public:
	
 
	QStringList getAURHelperCommands(QString AURHelper);
	QStringList updates;

signals:
    void readCheckUpdatesSignal ( QStringList &results );
	void promptPassword();
public slots:
    void checkUpdates ( bool namesOnly, bool aur );
	void getAURHelper(bool konsoleFlag, bool aur);
    int upgradeSystem ( bool konsoleFlag, bool aur, QString AURHelper );
	
};
#endif
