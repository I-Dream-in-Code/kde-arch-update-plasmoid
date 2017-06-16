import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import org.kde.plasma.core
ApplicationWindow{

    Image {
        id: archLogo
        width: 512
        height: 512
        source: "../images/archLogo.png"

    }

    Text {
        id: updateCount
        x: 294
        y: 0
        width: 242
        height: 172
        text: backend.getSize()
        font.weight: Font.ExtraBold
        font.bold: true

        font.pixelSize: 110
    }
}
