import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
	width: 600
	height: 300
	Text {
		id: settingsHeader
				x: 220
				y: 16
				width: 200

				text: qsTr("Settings")
				font.pixelSize: 36
			}

			GridLayout {
				id: grid
				columns: 2
				x: parent.width / 2 - width / 2
				y: parent.height / 2 - height / 2
				width: 300
				height: 200

				Text {
					id: intervalText
					width: 25
					height: 24
					text: qsTr("Check Updates Interval")
					font.pixelSize: 21
				}

				TextInput {
					id: textInput
					width: 80
					height: 20
					text: qsTr("15")
					validator: IntValidator {
					}
					selectionColor: "#00801c"
					renderType: Text.NativeRendering
					font.pixelSize: 21
				}

				Text {
					id: namesOnlyText
					text: qsTr("Package Names Only")
					font.pixelSize: 12
				}

				CheckBox {
				}
			}

			Image {
				id: backButton
				x: 32
				y: 234
				width: 55
				height: 60
				source: "qrc:/images/images/backButton.png"
			}
		}
