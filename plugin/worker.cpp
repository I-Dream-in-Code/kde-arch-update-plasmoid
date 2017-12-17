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





QString Worker::getAURHelper()
{
	//list all AUR helpers that are installed from /usr/bin
	QDir usrBin("/usr/bin");
	usrBin.setFilter(QDir::Files);
	QStringList aurHelperFilters;
	aurHelperFilters << "apacman" << "aura" << "aurget" << "bauerbill" << "cower" << "pacaur" << "pacget" << "packer" << "pkgbuilder" << "spinach" << "trizen" << "wrapaur" << "yaourt" << "yay";
	QStringList aurHelperList = usrBin.entryList(aurHelperFilters);
	qDebug() << "AUR HELPER LIST" << endl << aurHelperList;
	//sort
	qSort(aurHelperList.begin(), aurHelperList.end());

	//pacaur depends on cower but if only pacaur and cower are installed the first entry will always cower so return pacaur
	if (aurHelperList.size() == 2 && aurHelperList[0] == "cower" && aurHelperList[1] == "pacaur")
		return "pacaur";

	else
	{
		qDebug() << aurHelperList[0];
		return aurHelperList[0];
	}

	//return first alphabetically list aurhelper
}


QStringList Worker::getAURHelperCommands(QString AURHelper)
{
	QStringList arguments;

	if (AURHelper == "apacman")
	{
		arguments << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "aura")
	{
		arguments << "-Syyu" << "--noconfirm";
		return arguments;
	}

	//aurget only upgrades aur need to run pacman too
	else if (AURHelper == "aurget")
	{
		arguments << "-Syu" << "--noconfirm" << "&&" << "pacman" << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "bauerbill")
	{
		arguments << "-Syyu" << "--aur" << "--noconfirm";
		return arguments;
	}

	//burgaur only upgrades aur need to run pacman too
	else if (AURHelper == "burgaur")
	{
		arguments << "-su" << "--noconfirm" << "&&" << "pacman" << "-Syyu" << "--noconfirm";
		return arguments;
	}

	//cower on upgrades aur need to run pacman too
	else if (AURHelper == "cower")
	{
		arguments << "-ud" << "--noconfirm" << "&&" << "pacman" << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "pacaur")
	{
		arguments  << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "pacget")
	{
		arguments  << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "packer")
	{
		arguments << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "pkgbuilder")
	{
		arguments << "-Syyu" << "--noconfirm";
		return arguments;
	}

	//spinach only does AUR need to run pacman too
	else if (AURHelper == "spinach")
	{
		arguments << "-u" << "--noconfirm" << "&&" << "pacman" << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "trizen")
	{
		arguments << "-Syyu" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "wrapaur")
	{
		arguments << "-u" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "yaourt")
	{
		arguments << "-Syua" << "--noconfirm";
		return arguments;
	}

	else if (AURHelper == "yay")
	{
		arguments << "-Syyu" << "--noconfirm";
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
					aurPackages = checkUpdatesAURProcess.readAllStandardOutput();
// 			qDebug() <<aurPackages;
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

	//waits until started 3000 msec timeout
	if (checkUpdatesProcess.waitForStarted(-1))
	{
		qDebug() << "org.kde.archupdate: check updates started" << endl;

		//DEBUGGING WILL REMOVE
		if (checkUpdatesProcess.waitForReadyRead(-1))
		{
			checkUpdatesProcess.waitForFinished(-1);
			QString results = checkUpdatesProcess.readAllStandardOutput();
			qDebug() << "org.kde.archUpdate:  ================CHECKUPDATES CALL===================" << results;
			//split into vector by \n
			QStringList resultsVector = results.split(((QRegExp) "\n"));
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
			QStringList namesOnlyResults;

			if (namesOnly)
			{
				QVector<QStringList> tmp;

				for (int i = 0; i < resultsVector.length(); i++)
					tmp.push_back(resultsVector[i].split((QRegExp) " "));

				for (int i = 0; i < tmp.length(); i++)
					namesOnlyResults.push_back(tmp[i][0]);

				qDebug() << "org.kde.archUpdate:  ==========NAMES ONLY================" << namesOnlyResults;
				this->updates = namesOnlyResults;
			}

			else
				this->updates = resultsVector;
		}

		else
		{
			qDebug() << "org.kde.archUpdate: Your system is up to date, checkupdates returned nothing";
			QStringList err = QStringList();
			this->updates = err;
		}
	}

	else
	{
		qDebug() << "org.kde.archUpdate: Cannot start /usr/bin/checkupdates" << endl;
		qDebug() << checkUpdatesProcess.error() << endl << checkUpdatesProcess.errorString();
		QStringList err = QStringList();
		err << "cannot start checkupdates";
		this->updates = err;
	}
};


void Worker::upgradeSystem(bool konsoleFlag, bool aur, QString password)
{
	QProcess systemUpdateProcess;
	QString AURHelper = getAURHelper();

	if (konsoleFlag && aur)
	{
		QStringList arguments;
		// start with konsole --hold -e sudo **aur helper**
		arguments << "--hold" << "-e" << AURHelper;
		//add to arguments aur helper specific command to update
		// apacman is -Syu versus yaort is -Syua etc
		QStringList AURCommands = getAURHelperCommands(AURHelper);

		for (int i = 0; i < AURCommands.size(); i++)
		{
			arguments << AURCommands[i];
		}

		qDebug() << "KONSOLE ARGMUMENTS" << arguments;
		//start system update process for konsole
		systemUpdateProcess.start("/usr/bin/konsole", arguments);
// 				connect(systemUpdateProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(showProgressInqDebug()));
		qDebug() << "KONSOLE FLAG" << konsoleFlag;
	}

	//if user selects show in konsole in settings display in konsole
	else if (konsoleFlag && aur == false)
	{
		QStringList arguments;
		arguments << "--hold" << "-e" << "sudo" << "pacman" << "-Syyu" << "--noconfirm";
		systemUpdateProcess.start("/usr/bin/konsole", arguments);
	}

	else if (aur && konsoleFlag == false)
	{
		

		QStringList arguments;

		//start with aur helper add aur helper specific commands

		QStringList AURCommands = getAURHelperCommands(AURHelper);
		qDebug() << "AUR COMMANDS" << AURCommands;

		for (int i = 0; i < AURCommands.size(); i++)
		{
			arguments << AURCommands[i];
		}

		qDebug() << "ARGUMENTS" << arguments;
		systemUpdateProcess.start(AURHelper, arguments);
	}

	else
	{
		QStringList arguments;
		arguments << "/usr/bin/pacman" << "-Syyu" << "--noconfirm";
		//if user does not select show in konsole run pexec
		{
			systemUpdateProcess.start("pkexec", arguments);
		}
	}

	// calls Qprocess for "pexec pacman -Syu --noconfirm --force"

	//wait to start 3000 msec timeout
	if (systemUpdateProcess.waitForStarted(-1))
	{
		qDebug() << "STARTED";

		//wait for finish no timeout
		if (systemUpdateProcess.waitForReadyRead(-1))
		{
			
				QString inputLogCheck = systemUpdateProcess.readAllStandardOutput();
				qDebug() << "INPUT LOG CHECK" << endl << inputLogCheck;
				if (inputLogCheck.indexOf("password"))
				{
					systemUpdateProcess.write(password.toStdString().c_str());
				}
			

			if (systemUpdateProcess.waitForFinished(-1))
			{
				QString log = systemUpdateProcess.readAllStandardOutput();

				if (log.indexOf("file exists") != -1)
				{
					this->updates = QStringList();
					this->updates << "Cannot update file exists";
				}

				else if (log.indexOf("no space left on device") != -1)
				{
					qDebug() << "org.kde.archUpdate: no space left on device";
					this->updates = QStringList();
					this->updates << "No Space Left on Device.";
				}

				else
				{
					qDebug() << "org.kde.archUpdate: update complete";
					this->updates = QStringList();
				}
			}

			else
			{
				qDebug() << "org.kde.archUpdate:  cannot finish update";
				this->updates = QStringList();
				this->updates << "cannot finish check updates";
			}
		}

		else
		{
			qDebug() << "Cannot read from upgrade process";
		}
	}

	else
	{
		qDebug() << "org.kde.archUpdate: Cannot start system upgrade process";
		this->updates = QStringList();
		this->updates << "cannot start system upgrade process";
	}
}



