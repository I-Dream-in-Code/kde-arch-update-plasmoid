import QtQuick 2.2
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons
import org.kde.private.archUpdate 1.0

MouseArea {
	property int checkInterval: plasmoid.configuration.checkInterval * 60000
	onCheckIntervalChanged: function () {
		refreshTimer.interval = checkInterval
	}

	SystemCalls {
		id: backend
	}
	id: panelIconWidget
	anchors.fill: parent
	onClicked: plasmoid.expanded = !plasmoid.expanded

	PlasmaCore.IconItem {

		id: archUpdateIcon
		source: "/usr/share/icons/archUpdateNotifier/base/48x48/apps/archLogo.png"
		anchors.centerIn: parent
		width: units.roundToIconSize(Math.min(parent.width, parent.height))
		height: width
		//plasmoid.SizePolicy.horizontalSizePolicy:Fixed
		//plasmoid.SizePolicy.verticalSizePolicy:Fixed
		Rectangle {
			id: circle
			width: 15
			height: width
			radius: Math.round(width / 2)
			color: "Black"
			opacity: 0.7
			visible: main.updatesPending > 0
			anchors {
				right: parent.right
				top: parent.top
			}
			Text {
				text: main.updatesPending > 99
					  || main.updatesPending < 0 ? "99+" : main.updatesPending
				font.pointSize: 6
				color: "White"
				anchors.centerIn: circle
				visible: circle.visible
			}
		}
	}

	Timer {
		id: refreshTimer
		interval: checkInterval
		running: true
		repeat: true
		onTriggered: main.refresh(false)
	}

}
