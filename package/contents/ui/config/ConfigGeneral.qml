import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras

Item {

	property alias cfg_checkInterval: checkInterval.value
	property alias cfg_hideVersion: hideVersionCheckBox.checked
	property alias cfg_konsoleFlag: konsoleCheckBox.checked
	property alias cfg_aurSupportFlag: aurSupportCheckBox.checked
	property alias cfg_noConfirmAURFlag: noConfirmAURCheckBox.checked
	GridLayout {
		id: firstGrid

		Layout.fillWidth: true
		rowSpacing: 10
		columnSpacing: 10
		columns: 2

		Label {
			text: i18n("Check Every (Minutes)")
//			color: theme.textColor
		}

		SpinBox {
			id: checkInterval
			decimals: 0
			value: 15
			minimumValue: 1
			maximumValue: 999
			onValueChanged: cfg_checkInterval = value
		}
	}

	GridLayout {
		id: checkboxGroup
		Layout.fillWidth: true
		rowSpacing: 10
		columnSpacing: 10
		columns: 2
		anchors {
			top: firstGrid.bottom
			topMargin: Math.round(units.gridUnit)
		}
		Label {
			text: i18n("Hide the updates version number")
//			color: theme.textColor
		}
		CheckBox {
			id: hideVersionCheckBox
		}
		Label {
			text: i18n("Show upgrade on konsole")
//			color: theme.textColor
		}
		CheckBox {
			id: konsoleCheckBox
		}
		Label {
			anchors{
				top:konsoleCheckBox.bottom + .5
			}

			id: aurText
			text: i18n("AUR support")
//			color: theme.textColor
		}
		GridLayout {
			anchors{
				top:aurText.top
			}

			Layout.fillWidth: true
			rowSpacing: 10
			columnSpacing: 10
			columns: 2
			CheckBox {
				id: aurSupportCheckBox
			}

			Label {
				text: i18n("Enabled") + "  ("+ i18n("Requires ") + "checkupdates-aur)"
//				color: theme.textColor
			}
			CheckBox {
				id: noConfirmAURCheckBox
			}
			Label {
				text: "--noconfirm aur"
//				color: theme.textColor
			}
		}
	}
}
