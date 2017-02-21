import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1
import "./components"

ApplicationWindow 
{
    id: window
    visible: true
    width: 1280
    height: 800

    Action
    {
        id: updateAction
        text: "Update image"
        shortcut: "U"
        onTriggered:
        {
            image.source = "./../images/computing.png";
            image.update();
            viewer.compute();
            image.source = "./../images/computed.png";
            image.update();
        }
    }

    //Main window
    Rectangle
    {
        id: main
        anchors.fill:parent
        color: "#808080"

        Image
        {
            id : image
            source: "./../images/default.png"
            anchors.fill: parent
        }

        ViewerContainer
        {
            id : viewer
            anchors.right: parent.right
            anchors.top: parent.top
        }
    }
}
