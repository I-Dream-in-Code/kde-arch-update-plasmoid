import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.XmlListModel 2.0
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons
import org.kde.private.archUpdate 1.0

Item {
	id: fullRep
	property bool discard: false
	width: theme.implicitWidth
	height: theme.implicitHeight



	SystemCalls {
		id: backend
	}

	PlasmaExtras.ScrollArea {
		id: updatesScroll

		verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn

		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
			bottom: buttonRow.top
		}

		ListView {
			id: updateListView
			anchors.fill: parent
			width: parent.width
			height: parent.height
			model: main.theModel
			delegate: Item {
				x: 5
				width: parent.width
				height: 40
				Row {
					id: row1
					spacing: 10
					Text {
						text: modelData
						font.bold: true
						anchors.verticalCenter: parent.verticalCenter
						color: theme.textColor
					}
				}
			}
			snapMode: ListView.SnapToItem
		}
	}
	RowLayout {
		id: buttonRow
		anchors {
			margins: units.smallSpacing
			bottom: parent.bottom
			horizontalCenter: parent.horizontalCenter
		}

		PlasmaComponents.Button {
			id: updateButton
			Layout.fillWidth: true
			text: i18n("Update System")
			onClicked: function () {

				console.log("KONSOLE FLAG" + plasmoid.configuration.konsoleFlag)
				console.log("AUR FLAG" + plasmoid.configuration.aurSupportFlag)
				console.log("NO COFIRM AUR" + plasmoid.configuration.noConfirmAURFlag)
				console.log("NO COFIRM AUR" + plasmoid.configuration.yakuakeFlag)
				backend.upgradeSystem(plasmoid.configuration.konsoleFlag,plasmoid.configuration.aurSupportFlag,plasmoid.configuration.noConfirmAURFlag,plasmoid.configuration.yakuakeFlag);

				main.updatesPending=0;
				main.theModel.clear();
			}
		}
		PlasmaComponents.Button {
			id: checkUpatesButton
			Layout.fillWidth: true
			text: i18n("Check for Updates")
			onClicked: function () {
				main.refresh()
			}
		}
	}
}
