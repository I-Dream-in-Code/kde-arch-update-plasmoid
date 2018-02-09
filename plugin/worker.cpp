#include "systemCalls.h"
#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include "worker.h"
#include <QDir>
#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3
#define NO_SPACE_LEFT 4




QMutex Worker::mutex;
bool Worker::upgradeProcessRunning;

QString Worker::getAURHelper()
{
	QDir usrBin("/usr/bin");
	usrBin.setFilter(QDir::Files);
	QStringList aurHelperFilters;
	aurHelperFilters << "apacman" << "aura" << "aurget" << "bauerbill" << "cower" << "pacaur" << "pacget" << "packer" << "pkgbuilder" << "spinach" << "trizen" << "wrapaur" << "yaourt" << "yay";
	QStringList aurHelperList = usrBin.entryList(aurHelperFilters);
	qSort(aurHelperList);
	qDebug() << "AUR HELPER LIST" << endl << aurHelperList;

	//pacaur has cower dependecy and will always return cower if only pacaur is install so return pacaur
	if (aurHelperList.indexOf("pacaur") != -1)
		return "pacaur";

	else
		return aurHelperList[0];
}


QStringList Worker::getAURHelperCommands(QString AURHelper)
{
	QStringList arguments;

	if (AURHelper == "apacman")
	{
		arguments << "apacman" << "-Syu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "aura")
	{
		arguments << "sudo" << "aura" << "-Ayu" << "--noconfirm" << "; " << "sudo" << "pacman" << "-Syu" << "--noconfirm";
		return arguments;
	}

	//aurget only upgrades aur need to run pacman too
	else if (AURHelper == "aurget")
	{
		arguments << "aurget" << "-Syu" << "--noconfirm" << " ; " << "sudo" << "pacman" << "-Syu" << "--noconfirm";
		return arguments;;
		return arguments;
	}

	else if (AURHelper == "bauerbill")
	{
		arguments << "bauerbill" << "-Syu" << "--aur" << "--noconfirm";
		return arguments;
	}

	//burgaur only upgrades aur need to run pacman too
	else if (AURHelper == "burgaur")
	{
		arguments << "burgaur" << "-su" << "--noconfirm" << "; " << "sudo" << "pacman" << "-Syu" << "--noconfirm";
		return arguments;;
		return arguments;
	}

	//cower on upgrades aur need to run pacman too
	else if (AURHelper == "cower")
	{
		arguments << "cower" << "-ud" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "pacaur")
	{
		arguments << "pacaur" << "-Syu" << "--noconfirm" << "--noedit";
		return arguments;
	}

	else if (AURHelper == "pacget")
	{
		arguments << "pacget" << "-Syu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "packer")
	{
		arguments << "packer" << "-Syu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "pkgbuilder")
	{
		arguments << "pkgbuilder" << "-Syu" << "--noconfirm";
		return arguments;
	}

	//spinach only does AUR need to run pacman too
	else if (AURHelper == "spinach")
	{
		arguments << "spinach" << "-u" << "--noconfirm" << "; " << "sudo" << "pacman" << "-Syu" << "--noconfirm";
		return arguments;;
		return arguments;
	}

	else if (AURHelper == "trizen")
	{
		arguments << "trizen" << "-Syu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "wrapaur")
	{
		arguments << "wrapaur" << "-u" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "yaourt")
	{
		arguments << "yaourt" << "-Syua" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "yay")
	{
		arguments << "yay" << "-Syu" << "--noconfirm";
		return arguments;
	}

	else
	{
		return QStringList();
	}
}

void Worker::checkUpdates(bool namesOnly, bool aur)
{
	QString aurPackages;
	QStringList aurResultsVector;
	qDebug() << "clicked" << endl;
	//starts checkupdates as new qProcess
	QProcess checkUpdatesProcess;
	QStringList namesOnlyResults;
	QStringList resultsVector;
	QVector<QStringList> tmp;

	if (aur)
	{
		qDebug() << "=========== AUR ===========";
		QProcess checkUpdatesAURProcess;
		checkUpdatesAURProcess.start("/usr/bin/checkupdates-aur");

		if (checkUpdatesAURProcess.waitForStarted(-1))
		{
			if (checkUpdatesAURProcess.waitForReadyRead(-1))
			{
				if (checkUpdatesAURProcess.waitForFinished(-1))
				{
					//get all updates, split by new line for individual packages and convert to vector
					//remove trailing ""
					aurPackages = checkUpdatesAURProcess.readAllStandardOutput();
					aurResultsVector = aurPackages.split(((QRegExp) "\n"));
					aurResultsVector.removeAt(aurResultsVector.length() - 1);
					qDebug() << aurResultsVector;
				}

				else
				{
					qDebug() << "org.kde.archupdate: Cannot finish checkupdates-aur";
				}
			}

			else
			{
				qDebug() << "org.kde.archupdate: AUR returned nothing.  AUR is up to date. :)";
				//nothing is returned no AUR updates
				//do nothing
			}
		}

		else
		{
			qDebug() << "org.kde.archupdate: cannot start checkupdates-aur";
		}
	}

	checkUpdatesProcess.start("/usr/bin/checkupdates");

	if (checkUpdatesProcess.waitForStarted(-1))
	{
		qDebug() << "org.kde.archupdate: check updates started" << endl;

		if (checkUpdatesProcess.waitForReadyRead(-1))
		{
			checkUpdatesProcess.waitForFinished(-1);
			QString results = checkUpdatesProcess.readAllStandardOutput();
			qDebug() << "org.kde.archUpdate:  ================CHECKUPDATES CALL===================" << results;
			//split into vector by \n to get individual packages and create vector
			resultsVector = results.split(((QRegExp) "\n"));
			//remove trailing ""
			resultsVector.removeAt(resultsVector.length() - 1);

			//add aur packages
			for (int i = 0; i < aurResultsVector.length(); i++)
			{
				resultsVector.push_back(aurResultsVector[i]);
			}

			//sort vector so aur packages aren't at the bottom
			qSort(resultsVector.begin(), resultsVector.end());
			qDebug() << "org.kde.archUpdate:  =========CHECK UPDATES SPLIT============" << resultsVector;

			//if namesOnly is supplied as argument, return only package names without version upgrade information
			if (namesOnly)
			{
				for (int i = 0; i < resultsVector.length(); i++)
					tmp.push_back(resultsVector[i].split((QRegExp) " "));

				for (int i = 0; i < tmp.length(); i++)
					namesOnlyResults.push_back(tmp[i][0]);

				qDebug() << "org.kde.archUpdate:  ==========NAMES ONLY================" << namesOnlyResults;
				this->updates = namesOnlyResults;
				this->mutex.unlock();
			}

			//nameOnly false so return with version numbers
			else
			{
				this->updates = resultsVector;
				this->mutex.unlock();
			}
		}

		else
		{
			qDebug() << "org.kde.archUpdate: Your system is up to date, checkupdates returned nothing";
			QStringList err = QStringList();
			this->updates = err;

			if (aurResultsVector.size() == 0)
			{
				qDebug() << "org.kde.archUpdate: Your system is up to date, checkupdates and checkupdates-aur returned nothing";
				QStringList err = QStringList();
				this->updates = err;
				this->mutex.unlock();
			}

			//checkupdates returns nothing but checkupdates-aur returned to this->updates= checkupdates-aur
			else
			{
				for (int i = 0; i < aurResultsVector.length(); i++)
				{
					resultsVector.push_back(aurResultsVector[i]);
				}

				//sort vector so aur packages aren't at the bottom
				qSort(resultsVector.begin(), resultsVector.end());
				qDebug() << "org.kde.archUpdate:  =========CHECK UPDATES SPLIT============" << resultsVector;

				//if namesOnly is supplied as argument, return aur only package names without version upgrade information
				if (namesOnly)
				{
					QVector<QStringList> tmp;

					for (int i = 0; i < resultsVector.length(); i++)
						tmp.push_back(resultsVector[i].split((QRegExp) " "));

					for (int i = 0; i < tmp.length(); i++)
						namesOnlyResults.push_back(tmp[i][0]);

					qDebug() << "org.kde.archUpdate:  ========== AUR NAMES ONLY================" << namesOnlyResults;
					this->updates = namesOnlyResults;
					this->mutex.unlock();
				}

				// return checkupdates-aur with version numbers
				else
				{
					this->updates = resultsVector;
					this->mutex.unlock();
				}

				qDebug() << "org.kde.archUpdate: checkupdates returned nothing but AUR packages need upgrade.";
			}
		}
	}

	else
	{
		qDebug() << "org.kde.archUpdate: Cannot start /usr/bin/checkupdates" << endl;
		qDebug() << checkUpdatesProcess.error() << endl << checkUpdatesProcess.errorString();
		QStringList err = QStringList();
		err << "cannot start checkupdates";
		this->updates = err;
		this->mutex.unlock();
	}
};


void Worker::upgradeSystem(bool konsoleFlag, bool aur, bool noconfirm)
{
	QProcess systemUpdateProcess;


	//only display aur in konsole
	if (aur)
	{
		QString AURHelper = getAURHelper();
		//run /bin/bash -c konsole --hold -e 'sh -c " *aur helper commnads* ; echo Update Finished "
		QStringList arguments;
		arguments << "-c";
		// start with konsole  --hold -e  **aur helper**
		QString args = "konsole --hold -e 'sh -c \" ";

		
		//add to arguments aur helper specific command to update
		// apacman is -Syu versus yaort is -Syua etc
		qDebug() << "AUr hELPER======" << AURHelper;
		QStringList AURCommands = getAURHelperCommands(AURHelper);

		for (int i = 0; i < AURCommands.size(); i++)
		{
			args += AURCommands[i] + " ";
		}
		args += " ;  echo "" ; echo ---------------- ; echo Update Finished\"'";
		arguments << args;
		
		//remove --noconfirm if flag in settings not set
		if (noconfirm == false)
		{
			int indx = arguments.indexOf("--noconfirm");
			if(AURHelper=="pacaur") arguments.removeAt(arguments.indexOf("--noedit"));
			arguments.removeAt(indx);
		}

		//start system update process for konsole
		qDebug() << "AUR ARGS " << arguments;
		systemUpdateProcess.start("/bin/bash", arguments);
	}

	//if user selects show in konsole in settings display in konsole
	else if (konsoleFlag)
	{
		QStringList arguments;
		// /bin/bash -c konsole --hold -e 'sh -c "sudo pacman -Syu ; echo Update Finished'""
		arguments << "-c" << "konsole --hold -e 'sh -c \"sudo pacman -Syu ; echo "" ; echo ---------------- ; echo Update Finished \"'";
		qDebug() << "ARGS " << arguments;
		systemUpdateProcess.start("/bin/bash", arguments);
	}

	else
	{
		//pexec pacman -Syu --noconfirm
		QStringList arguments;
		arguments << "/usr/bin/pacman" << "-Syu" << "--noconfirm";
		//if user does not select show in konsole run pexec
		{
			systemUpdateProcess.start("pkexec", arguments);
		}
	}

	if (systemUpdateProcess.waitForStarted(-1))
	{
		qDebug() << "STARTED";

		
			if (systemUpdateProcess.waitForFinished(-1)) ;

			else
			{
				qDebug() << "org.kde.archUpdate:  cannot finish update";
				this->mutex.unlock();
				this->upgradeProcessRunning = false;
			}
		
			this->mutex.unlock();
			this->upgradeProcessRunning = false;
	}

	else
	{
		qDebug() << "org.kde.archUpdate: Cannot start system upgrade process";

		this->mutex.unlock();
		this->upgradeProcessRunning = false;
	}
};
