import QtQuick 2.7
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1
import RTApplicationComponents 1.0

import "./components"

Item
{
    id: window
    visible: true
    width: 1280
    height: 800

    Viewer
    {
        id:viewer
        focus: true
    }

    Shortcut
    {
        id: updateAction
        sequence: StandardKey.Refresh
        onActivated:
        {
            viewer.setBackgroundImage(":/graphical-assets/computing")
            viewer.compute();
        }
    }

    Shortcut
    {
        id:saveAction
        sequence: StandardKey.Save
        onActivated:
        {
            viewer.save()
        }
    }

    Text {
        id: label
        style: Text.Raised
        color: "black"
        styleColor: "white"
        wrapMode: Text.WordWrap
        text:  updateAction.nativeText + " to compute the image.\n" + saveAction.nativeText + " to save the image."
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }


}
