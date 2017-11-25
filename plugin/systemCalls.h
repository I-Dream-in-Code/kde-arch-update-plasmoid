#include <qt/QtCore/QStringList>
#ifndef SYSTEMCALLS_H
#define SYSTEMCALLS_H
#include <QProcess>
#include <QtConcurrent/QtConcurrentRun>
#include "worker.h"
/**
 * @brief The systemCalls class
 * Detailed: backend system calls for widget specifically "checkupdates" and "pexec pacman -Syu'
 * @author: Mike Harris
 * @date: May 2017
*/

class systemCalls : public QObject
{
    Q_OBJECT
    QThread workerThread;
    Worker *worker;
public:
    /**
    * @brief systemCalls default contructor
    * @param parent
    */
    explicit systemCalls ( QObject *parent = 0 );

    ~systemCalls();

    Q_INVOKABLE QStringList checkUpdates ( bool namesOnly, bool aur );
	Q_INVOKABLE int upgradeSystem(bool konsoleFlag, bool aur, QString AUR);

    bool isConnectedToNetwork();
signals:
    /**
    @brief starts checkupdates on worker thread eventually returns worker->updates
    @return QStringList of stdout output of checkupdates
    @details emits signal for worker thread to run checkupdates which saves checkupdates results in worker->updates
    @param arguments- bool:namesOnly to strip version numbers, bool: aur to show AUR updates
    */
    QStringList checkUpdatesSignal ( bool namesOnly,bool aur );
	/**
	 @brief starts upgradeSystem on worker thread
	 @return int for exit code
	 @details upgrades system. if konsoleflag=true show updates in console. if aur is true run AUR helper and update AUR packages
	 */
    int upgradeSystemSignal ( bool konsoleFlag, bool aur , QString AURHelper);
public slots:

	/**
	 @brief returns worker->updates
	 */
    QStringList readCheckUpdates();
};

#endif // SYSTEMCALLS_H
