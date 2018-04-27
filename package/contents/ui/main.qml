import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.private.archUpdate 1.0

Item {
	SystemCalls {
		id: backend
	}
	id: main
	property string appName: "Arch Updater"
	property string version: "1.0"
	property int iconSize: units.iconSizes.smallMedium
	property int leftColumnWidth: iconSize + Math.round(units.gridUnit / 2)
	property string appletIcon: "archLogo.png"
	property int updatesPending: 0
	property var theModel: updateListModel
	property var namesOnly: plasmoid.configuration.hideVersion
	property var aurSupport: plasmoid.configuration.aurSupportFlag
    property bool internetCheck: false
	onNamesOnlyChanged: timer.restart()
	onAurSupportChanged: timer.restart()
	Plasmoid.icon: plasmoid.file("images", appletIcon)

	Plasmoid.compactRepresentation: CompactRepresentation {}

	Plasmoid.fullRepresentation: FullRepresentation {}

    Plasmoid.status: {
        if(updatesPending > 0) {
            return PlasmaCore.Types.ActiveStatus;
        }

        return PlasmaCore.Types.PassiveStatus;
    }

	PlasmaCore.DataSource {
		id: notificationSource
		engine: "notifications"
		connectedSources: "org.freedesktop.Notifications"
	}
	ListModel {
		id: updateListModel


	}

	//used to initialize start up
	Timer {
		id: timer
		interval: 1000
		running: true
		repeat: false
		onTriggered: refresh()
	}


    Timer{
        id: noInternetRecheckTimer
        interval: 60000
        repeat: false
        onTriggered: {

            console.log("org.kde.archUpdate: rechecking internet");
            refresh()
        }

    }

	function refresh() {

            console.log("org.kde.archUpdate: checking internet")
            if(!backend.isConnectedToNetwork() && internetCheck==false){
                noInternetRecheckTimer.start();
                console.log("org.kde.archUpdate: Timer started");
                internetCheck=true;
                return;
            }

            if(internetCheck){
                console.log("org.kde.archUpdate: still no internet connection");
                updateListModel.clear();
                updatesPending=1;
                updateListModel.append({"text":"No internet connection"});
                internetCheck=false;
                return;
            }
			updateListModel.clear();
			var packageList;
			console.log("NAMES ONLY " + plasmoid.configuration.hideVersion);
			console.log("AUR SUPORT" + plasmoid.configuration.aurSupportFlag);
			//logic to show either names only, AUR, both or none
			backend.checkUpdates(plasmoid.configuration.hideVersion,plasmoid.configuration.aurSupportFlag);
			//append packages to full representation list
			packageList=backend.readCheckUpdates();
			for (var i = 0; i < packageList.length; i++) {
				updateListModel.append({"text": packageList[i]});
			}
			//counter on CompactRepresentation
			updatesPending = packageList.length;
		}
}
