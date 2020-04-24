import QtQuick 2.0

Item{
    id:mainItem
    property int llX: 0
    property int llY: 0
    property alias mouseArea: glForm.mouseArea
    property alias myGLItem: glForm.myGLItem

    Rectangle{
        id: backgroundRect
        color: "#ff0000"
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightblue" }
            GradientStop { position: 0.33; color: "darkgrey" }
            GradientStop { position: 1.0; color: "darkgrey" }
        }
    }
    GLForm {
        id:glForm
        anchors.fill: parent
        llX: mainItem.llX + x
        llY: mainItem.llY + mainItem.height - y - height

        Text {
            text: "Nächster Spieler: Rot"
            id: statusText
        }

        buttonLeft.onClicked: myGLItem.rotateLeft()
        buttonStop.onClicked: myGLItem.stopRotation()
        buttonRight.onClicked: myGLItem.rotateRight()
        mouseArea.onPressed: {
            console.log("Mouse pressed at: " + mouse.x + "/" + mouse.y)
            myGLItem.mousePressed(mouse.x, mouse.y, mouse.button)
            mouse.accepted = true
            statusText.text = myGLItem.getStatusText();
        }
        mouseArea.onReleased: {
            console.log("Mouse released at: " + mouse.x + "/" + mouse.y)
            myGLItem.mouseReleased(mouse.x, mouse.y, mouse.button)
            mouse.accepted = true
            statusText.text = myGLItem.getStatusText();
        }
        mouseArea.onPositionChanged:  {
            console.log("Mouse moved to: " + mouse.x + "/" + mouse.y)
            myGLItem.mouseMoved(mouse.x, mouse.y, mouse.button)
            mouse.accepted = true
            statusText.text = myGLItem.getStatusText();
        }
        Keys.onLeftPressed: {
            myGLItem.rotateY(-5.0)
        }
        Keys.onRightPressed: {
            myGLItem.rotateY(5.0)
        }
        Keys.onUpPressed: {
            if(event.modifiers & Qt.ShiftModifier)
              myGLItem.zoomIn()
            else myGLItem.rotateX(5.0)
        }
        Keys.onDownPressed: {
            if(event.modifiers & Qt.ShiftModifier)
              myGLItem.zoomOut()
            else myGLItem.rotateX(-5.0)
        }
    }
    Component.onCompleted: console.log("buttonLeft.height " + glForm.buttonLeft.height + " lly " + llY)
}
