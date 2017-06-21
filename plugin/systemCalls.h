#ifndef SYSTEMCALLS_H
#define SYSTEMCALLS_H
#include <QProcess>
#include <QtConcurrent/QtConcurrentRun>
/**
 * @brief The systemCalls class
 * Detailed: backend system calls for widget specifically "checkupdates" and "pexec pacman -Syu'
 * @author: Mike Harris
 * @date: May 2017
*/

class systemCalls : public QObject
{
    Q_OBJECT
public:

    int size;
    /**
    * @brief systemCalls default contructor
    * @param parent
    */
    explicit systemCalls(QObject *parent = 0);
    /**
    @brief calls and returns output of checkupdates
    @return QStringList of stdout output of checkupdates
      @details the possible output of the Qstring is **package_name** **current_version** -> **new_version** or simply **package_name** with arguments set to "namesOnly"
      @param arguments- only option is "namesOnly"
      */

	bool iscConnectedToNetwork();
    Q_INVOKABLE QStringList checkUpdates(QString arguments);
    /**
      @brief upgrades system
      @return exit code
      @details calls "pexec pacman -Syu --noconfirm" and prompts for password then updates system
      */
	
	int upgradeSystem();

	Q_INVOKABLE void upgradeConcurrent();
	/**
	 * @brief used to call upgrade process on a different thread of the whole system hangs
	 * @return none
	 * @details uses QtConcurrent::Run to call system upgrade
	 * */
    
	
	Q_INVOKABLE int getSize(){return size;}
    

    QProcess *checkUpdatesProcess; 	/**<
@brief used for QProcess in checkUpdates call
        */

    QProcess *systemUpdateProcess;/**<
@brief used for QProcess in upgradeSystem call*/

signals:

public slots:
    /**
    @brief slot for debugging will be removed*/
    void showProgressInqDebug();
};

#endif // SYSTEMCALLS_H