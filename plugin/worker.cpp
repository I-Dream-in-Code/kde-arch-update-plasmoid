#include "systemCalls.h"
#include <QString>
#include <QVector>
#include <QDebug>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>
#include "worker.h"
#include <QDir>
#include <QThread>
#define SUCCESS 0
#define CANNOT_START 1
#define NO_INTERNET 2
#define CONFLICTS 3
#define NO_SPACE_LEFT 4

bool Worker::mutex;
bool Worker::upgradeProcessRunning;

QString Worker::getAURHelper()
{
	QDir usrBin("/usr/bin");
	usrBin.setFilter(QDir::Files);
	QStringList aurHelperFilters;
	aurHelperFilters << "apacman" << "aura" << "aurget" << "aurman" << "bauerbill" << "cower" << "pacaur" << "pacget" << "packer" << "pakku" << "pikaur" << "pkgbuilder" << "naaman" << "spinach" << "trizen" << "wrapaur" << "yaourt" << "yay";
	QStringList aurHelperList = usrBin.entryList(aurHelperFilters);
	qSort(aurHelperList);
	qDebug() << "AUR HELPER LIST" << endl << aurHelperList;

	//pacaur has cower dependecy and will always return cower if only pacaur is install so return pacaur
	if (aurHelperList.indexOf("pacaur") != -1)
		return "pacaur";

	else if (aurHelperList.length() == 0)
		return nullptr;
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
		
		return arguments;
	}

	else if (AURHelper == "bauerbill")
	{
		arguments << "bb-wrapper" << "-Syu" << "--aur" << "--noconfirm";
		return arguments;
	}

	//burgaur only upgrades aur need to run pacman too
	else if (AURHelper == "burgaur")
	{
		arguments << "burgaur" << "-su" << "--noconfirm" << "; " << "sudo" << "pacman" << "-Syu" << "--noconfirm";
		
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

	else if (AURHelper == "aurman")
	{
		arguments << "aurman" << "-Syu" << "--noconfirm" << "--noedit";
		return arguments;
	}
	else if (AURHelper == "pikaur")
	{
		arguments << "pikaur" << "-Syu" << "--noconfirm" << "--noedit";
		return arguments;
	}
	else if (AURHelper == "pakku")
	{
		arguments << "pakku" << "-Syu" << "--noconfirm";
		return arguments;
	}
	else if (AURHelper == "naaman")
	{
		arguments << "naaman" << "-Syu" << "--no-confirm";
		return arguments;
	}

	else
		return QStringList();
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
					qDebug() << "org.kde.archupdate: Cannot finish checkupdates-aur";
			}

			else
			{
				qDebug() << "org.kde.archupdate: AUR returned nothing.  AUR is up to date. :)";
				//nothing is returned no AUR updates
				//do nothing
			}
		}

		else
			qDebug() << "org.kde.archupdate: cannot start checkupdates-aur";
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
				resultsVector.push_back(aurResultsVector[i]);

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
				this->mutex = false;
			}

			//nameOnly false so return with version numbers
			else
			{
				this->updates = resultsVector;
				this->mutex = false;
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
				this->mutex = false;
			}

			//checkupdates returns nothing but checkupdates-aur returned to this->updates= checkupdates-aur
			else
			{
				for (int i = 0; i < aurResultsVector.length(); i++)
					resultsVector.push_back(aurResultsVector[i]);

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
					this->mutex = false;
				}

				// return checkupdates-aur with version numbers
				else
				{
					this->updates = resultsVector;
					this->mutex = false;
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
		this->mutex = false;
	}
};

void Worker::toggleYakuake(QString session)
{
	QString yakuakeSession = QString::number(session.toInt() - 1);
	QProcess raiseSession;
	QStringList raiseSessionArguments;
	raiseSessionArguments << "org.kde.yakuake" << "/yakuake/sessions" << "raiseSession" << yakuakeSession;
	raiseSession.start("qdbus-qt5", raiseSessionArguments);
	raiseSession.waitForFinished();
	QProcess toggleWindow;
	QStringList toggleWindowArguments;
	toggleWindowArguments << "org.kde.yakuake" << "/yakuake/window" << "toggleWindowState";
	toggleWindow.start("qdbus-qt5", toggleWindowArguments);
	toggleWindow.waitForFinished();
}

QString Worker::prepareYakuake()
{
	// check if yakuake already has a session "arch updater")
	QProcess terminalIdListProcess;
	QStringList args;
	args << "org.kde.yakuake" << "/yakuake/sessions" << "org.kde.yakuake.terminalIdList";
	terminalIdListProcess.start("qdbus-qt5", args);
	terminalIdListProcess.waitForFinished();
	QString terminalIds(terminalIdListProcess.readAllStandardOutput().simplified());
	QStringList terminalList = terminalIds.split(",");
	bool foundTab = false;
	QString terminal = "";
	QString session = "";

	foreach (const QString& str, terminalList)
	{
		QStringList arguments;
		arguments << "org.kde.yakuake" << "/yakuake/tabs" << "org.kde.yakuake.tabTitle" << str;
		QProcess getTitleProcess;
		getTitleProcess.start("qdbus-qt5", arguments);
		getTitleProcess.waitForFinished();
		QString tabTitle(getTitleProcess.readAllStandardOutput().simplified());
		if(tabTitle == "arch updater")
		{
			QProcess getSessionId;
			QStringList getSessionIdArguments;
			getSessionIdArguments << "org.kde.yakuake" << "/yakuake/sessions" << "sessionIdForTerminalId" << str;
			getSessionId.start("qdbus-qt5", getSessionIdArguments);
			getSessionId.waitForFinished();
			QString sessionId(getSessionId.readAllStandardOutput().simplified());
			terminal = str;
			session = QString::number(str.toInt() + 1);
			foundTab = true;
		}
	}

	// if the session does not exist, add it
	if (!foundTab)
	{
		QProcess addSessionProcess;
		addSessionProcess.start("qdbus-qt5 org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession");
		addSessionProcess.waitForFinished();
		terminal = addSessionProcess.readAllStandardOutput();
		QProcess setTitleProcess;
		QStringList arguments;
		arguments << "org.kde.yakuake" << "/yakuake/tabs" << "setTabTitle" << terminal << "arch updater";
		setTitleProcess.start("qdbus-qt5", arguments);
		setTitleProcess.waitForFinished();
		session = QString::number(terminal.toInt() + 1);
	}

	return session;
}


void Worker::upgradeSystem(bool konsoleFlag, bool aur, bool noconfirm, bool yakuakeFlag, bool orphan, bool snapRefreshFlag)
{
	/*
	 * Since QProcess causes a stack crash when stringing multiple commands we call a seperate shell script to call
	 * the update process.
	 *
	 * Since the update process takes multiple commands seperated by ; we need to have the commands separated by , to
	 * then create an IFS=',' in bash to get each command in an array.
	 *
	 *
	 * The first argument is either konsole, yakuake or pkexec
	 *
	 * Ex :
	 *      sudo pacman -Syu --noconfirm, echo update finished
	 *
	 *      in bash the array becomes
	 *      ("sudo pacman -Syu --noconfirm;", "echo update finished;")
	 *
	 * Then all the commands are concatintated back into a large string and called in the bash script
	 *
	 */


	QProcess systemUpdateProcess;

	//if yakuake is not running start and sleep for 2 seconds then call upgradeSystem again now that yakuake is started
	if (yakuakeFlag)
	{
		QProcess ps;
		QProcess grep;
		ps.setStandardOutputProcess(&grep);
		ps.start("ps cax");
		grep.start("grep yakuake");
		grep.setProcessChannelMode(QProcess::ForwardedChannels);
		ps.waitForStarted();
		bool retval = false;
		QByteArray buffer;

		while ((retval = grep.waitForFinished()));

		buffer.append(grep.readAll());

		// if yakuake is not running, start it


		if (buffer == "")
		{
			this->yakuakeProcess = new QProcess();
			this->yakuakeProcess->start("yakuake");
			this->yakuakeProcess->waitForStarted(-1);
			prepareYakuake();
			QThread::sleep(2);
			return upgradeSystem(konsoleFlag, aur, noconfirm, yakuakeFlag, orphan, snapRefreshFlag);
		}
	}


	if (aur)
	{
		QString AURHelper = getAURHelper();
		if(AURHelper == nullptr)
		{
			qDebug () << "org.kde.archUpdate: you have no AUR helper installed.  Please install an AUR helper or disable AUR support";
			return;
		}
		// add to arguments aur helper specific command to update
		// apacman is -Syu versus yaort is -Syua etc
		qDebug() << "AUR HELPER======" << AURHelper;
		QStringList AURCommands = getAURHelperCommands(AURHelper);
		QStringList arguments;


		//remove --noconfirm if flag in settings not set
		if (noconfirm == false)
		{
			for(int i = 0; i < AURCommands.size(); i++)
			{
				if(AURCommands[i] == "--noconfirm" || AURCommands[i] == "--no-confirm")
					AURCommands.removeAt(i);

				if (AURCommands[i] == "--noedit")
					AURCommands.removeAt(i);

			}
		}


		if (yakuakeFlag)
		{
			QString terminal = prepareYakuake();
			// 			arguments << "yakuake" << "org.kde.yakuake" << "/Sessions/" + terminal << "runCommand" ;
			arguments << "yakuake" << terminal;


			toggleYakuake(terminal);
		}

		else   // use Konsole
		{
			//run /bin/bash -c konsole --hold -e 'sh -c " *aur helper commnads* ; echo Update Finished "

			// start with konsole  --hold -e  **aur helper**
			arguments << "konsole";

		}
		for (int i = 0; i < AURCommands.size() - 1; i++)
			arguments << AURCommands[i];

		arguments << AURCommands[AURCommands.size() - 1] + ",";


		if(orphan)
			arguments << "echo," << "echo" << "Cleaning" << " Orphans," << "sudo" << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm,";

		if(snapRefreshFlag)
			arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

		arguments << "echo," << "echo" << "----------------," <<  "echo" << "Update" << "Finished";
		//start system update process
		qDebug() << "AUR ARGS " << arguments;
		systemUpdateProcess.start("/usr/bin/ArchUpdater", arguments);
	}

	else //no aur flag
	{
		// if user selects show in konsole in settings display in konsole
		if (konsoleFlag)
		{
			QStringList arguments;
			// /bin/bash -c konsole --hold -e 'sh -c "sudo pacman -Syu ; echo Update Finished'""
			arguments << "konsole" << "sudo" << "pacman" << "-Syu,";

			if(orphan)
				arguments << "echo," << "echo" << "Cleaning" << " Orphans," << "sudo" << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm,";

			if(snapRefreshFlag)
				arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

			arguments << "echo," << "echo" << "----------------," <<  "echo" << "Update" << "Finished";
			systemUpdateProcess.start("/usr/bin/ArchUpdater", arguments);
		}

		// if user selects show in yakuake in settings display in yakuake
		if (yakuakeFlag)
		{
			QStringList arguments;
			QString terminal = prepareYakuake();
			arguments << "yakuake" << terminal << "sudo" << "pacman" << "-Syu,";

			if(orphan)
				arguments << "echo," << "echo" << "Cleaning" << " Orphans," << "sudo" << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm,";

			if(snapRefreshFlag)
				arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

			arguments << "echo," << "echo" << "----------------," <<  "echo" << "Update" << "Finished";
			systemUpdateProcess.start("/usr/bin/ArchUpdater", arguments);
			qDebug() << "ARGS " << arguments;
			toggleYakuake(terminal);
		}

		//run in background
		if (yakuakeFlag == false && konsoleFlag == false)
		{
			//pexec pacman -Syu --noconfirm
			QStringList arguments;
			arguments << "pkexec" << "/usr/bin/pacman" << "-Syu" << "--noconfirm,";

			if(orphan)
				arguments << "pacman" << "-Rns" << "$(pacman -Qtdq)" << "--noconfirm";

			if(snapRefreshFlag)
				arguments << "echo," <<"echo" << "Updating" << "Snap" << "Packages," << "sudo" << "snap" << "refresh,";

			systemUpdateProcess.start("usr/bin/ArchUpdater", arguments);
		}
	}
	if (systemUpdateProcess.waitForStarted(-1))
	{

		if (systemUpdateProcess.waitForFinished(-1));

		else
		{
			qDebug() << "org.kde.archUpdate:  cannot finish update";
			this->mutex = false;
			delete this->yakuakeProcess;
			this->upgradeProcessRunning = false;
		}

		qDebug() << "org.kde.archUpdate: Upgrade process finished";
		this->mutex = false;
		this->upgradeProcessRunning = false;
	}

	else
	{
		qDebug() << "org.kde.archUpdate: Cannot start system upgrade process";
		this->mutex = false;
		delete this->yakuakeProcess;
		this->upgradeProcessRunning = false;
	}

};
