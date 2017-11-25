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
    void readCheckUpdatesSignal ( QStringList &results );
public slots:
    void checkUpdates ( bool namesOnly, bool aur );
    int upgradeSystem ( bool konsoleFlag, bool aur,QString AURHelper );
	
};
#endif
