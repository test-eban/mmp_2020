import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("4 Gewinnt")

        GLPage {
            id: glPage
            llX: glPage.x
            llY: tabBar.height
            anchors.fill: parent

             mouseArea.onClicked:{
                 var globalCoord = mapToItem(mainWindow.contentItem, mouse.x, mouse.y)
                 var windowLowerLeft = mapToItem(mainWindow.contentItem, 0, height)
                 var viewportMouseX = (windowLowerLeft.x + mouse.x) -  myGLItem.viewportX
                 var viewportMouseY = (mainWindow.height - mouse.y) - myGLItem.viewportY
                 console.log()
                 console.log("Mouse clicked at item coordinates: (" + mouse.x + ", " + mouse.y +")")
                 console.log("Mouse clicked at global coordinates: (" + globalCoord.x + ", " + mouse.y +")")
                 console.log("Window coordinates of lower left corner of GLPage: (" + windowLowerLeft.x + ", " + windowLowerLeft.y +")")
                 console.log("Mouse clicked at viewport coordinates: (" + viewportMouseX + ", " +  viewportMouseY + ")")
             }
        }



     Component.onCompleted: console.log("Screen.devicePixelRatio: " + screen.devicePixelRatio + " swipeView.height: " + swipeView.height)
}
