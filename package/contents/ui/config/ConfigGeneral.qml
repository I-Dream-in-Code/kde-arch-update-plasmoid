import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras

Item {

	   property alias cfg_checkInterval: checkInterval.value
	   property alias cfg_hideVersion: hideVersion.checked

	   GridLayout {
			   id: firstGrid

			   Layout.fillWidth: true
			   rowSpacing: 10
			   columnSpacing: 10
			   columns: 1

			   Text {
					   text: i18n("Check Every (Minutes)")
			   }

			   SpinBox {
					   id: checkInterval
					   decimals: 0
					   value: 15
					   minimumValue: 1
					   maximumValue: 999
					   onValueChanged: cfg_checkInterval=value
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
