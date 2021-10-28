import QtQuick 2.2
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons
import org.kde.private.archUpdate 1.0;


Item {

    id: compactRep
	property int checkInterval: plasmoid.configuration.checkInterval * 60000
	onCheckIntervalChanged: function(){refreshTimer.interval=checkInterval}
	height: units.iconSizes.toolbar
	width: units.iconSizes.toolbar
    SystemCalls {
        id: backend
    }
    
	Image {
        id: compactIcon
		source: "../images/chosen"
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        anchors.margins: units.smallSpacing
		width: parent.width
		height: parent.height

    }
	Timer {
		id: refreshTimer
		interval: checkInterval
		running: true
		repeat: true
		onTriggered: main.refresh()
	}
    Rectangle {
        id: circle
        width: 15
        height: width
        radius: Math.round(width / 2)
        color: "Black"
        opacity: 0.7
        visible: main.updatesPending > 0 || updatesPending==="?"
        anchors {
            right: parent.right
            top: parent.top
        }
    }

	Text {
		text: main.updatesPending > 99 || main.updatesPending<0 ? "99+" : main.updatesPending
		font.pointSize: 6
		color: "White"
		anchors.centerIn: circle
		visible: circle.visible
	}

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if (!fakeFullRepresentation.discard) {
                fakeFullRepresentation.discard = true;
		if (fakeFullRepresentation.listModel) {
                	fakeFullRepresentation.listModel.clear();
		}
            }
            plasmoid.expanded = !plasmoid.expanded;
        }
        hoverEnabled: true
    }

    FullRepresentation {
        id: fakeFullRepresentation
        visible: false
    }
}
