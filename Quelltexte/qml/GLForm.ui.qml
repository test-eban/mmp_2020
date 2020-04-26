import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import GLComponents 1.0

Item {
    id: page
    property alias myGLItem: myGLItem
    property alias buttonLeft: buttonLeft
    property alias buttonRight: buttonRight
    property alias buttonStop: buttonStop
    property alias mouseArea: mouseArea
    property int llX : 0
    property int llY : 0
    Rectangle{
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: rowLayout.top
        color: "transparent"
        border.color: "black"

    MyGLItem {
        id: myGLItem
        anchors.fill:parent
        viewportX: page.llX + x
        viewportY: screen.devicePixelRatio * (page.llY + rowLayout.height)
        MouseArea {
            id: mouseArea
            anchors.fill: parent
       }
    }
    }
    RowLayout {
        id: rowLayout
        width: 3 * buttonLeft.width + 4 * spacing
        height: buttonLeft.height
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            id: buttonLeft
            height: 80
            text: qsTr("Rotate Left")
        }
        Button {
            id: buttonStop
            height: 80
            text: qsTr("Stop")
        }
        Button {
            id: buttonRight
            height: 80
            text: qsTr("Rotate Right")
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
