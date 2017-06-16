import QtQuick 2.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import org.kde.plasma.core 2.0 as PlasmaCore

Item {

	property int refreshVal: parseInt(refresh.value)
	property alias hideVersion: hideVersion.checked

	GridLayout {
		id: firstGrid

		Layout.fillWidth: true
		rowSpacing: 10
		columnSpacing: 10
		columns: 1

		Text {
			text: i18n("Check Every (Minutes)")
			color: theme.textColor
		}

		SpinBox {
			id: refresh
			decimals: 0
			stepSize: 15
			value: 15
			minimumValue: 1
			maximumValue: 999
		}
	}

	Column {
		id: checkboxGroup
		Layout.fillWidth: true
		anchors {
			top: firstGrid.bottom
			topMargin: Math.round(units.gridUnit)
		}
		spacing: 10

		CheckBox {
			id: hideVersion
			text: i18n("Hide the updates version number")
		}
	}
}
