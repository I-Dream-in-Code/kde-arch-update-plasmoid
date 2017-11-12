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
	GridLayout {
		id: firstGrid

		Layout.fillWidth: true
		rowSpacing: 10
		columnSpacing: 10
		columns: 2

		Text {
			text: i18n("Check Every (Minutes)")
			color: theme.textColor
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
		Text {
			text: i18n("Hide the updates version number")
			color: theme.textColor
		}
		CheckBox {
			id: hideVersionCheckBox

		}
		Text {
			text: i18n("Show upgrade on konsole")
			color: theme.textColor
		}
		CheckBox{
			id: konsoleCheckBox

		}
		Text {
			text: i18n("AUR support")
			color: theme.textColor
		}
		CheckBox{
			id: aurSupportCheckBox
		}

	}
}
