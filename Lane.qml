import QtQuick 2.0
import QtQuick.Controls 2.12
import Line 1.0
import QtQuick.Layouts 1.12

Rectangle {
    id:lane_background

    width:height*0.7
    height:background.height*0.95
    //chart
    ScrollView{
        id:sv
        anchors.fill:parent
        clip:true
        focus: true
        focusPolicy: Qt.WheelFocus

        ListView{
            clip: true
            //spacing: (60/bpm*500)
            model: LineModel{
                mline: line
            }
            delegate:ButtonLine{
                width:parent.width
            }
        }
        /*
        Rectangle{
            anchors.bottom: parent.bottom
            id: chart1
            color:"gainsboro"
            height: 500//player.time/2
            width: parent.width
        }
        Rectangle{
            anchors.bottom: chart1.top
            id: chart2
            color:"red"
            height: 500
            width: parent.width
        }*/
    }/*
    RowLayout{
        anchors.bottom: parent.bottom
        width: parent.width
        Button{
            text:qsTr("Add new item")
            onClicked: line.appendItem()
            Layout.fillWidth: true
        }
        Button{
            text:qsTr("Removed Completed")
            onClicked: line.removeCompletedItems()
            Layout.fillWidth: true
        }
    }*/
    //left
    Rectangle{
        color: "darkblue"
        height: parent.height
        width: 5
        anchors.left: parent.left
    }
    //right
    Rectangle{
        color: "darkblue"
        height: parent.height
        width: 5
        anchors.right: parent.right
    }
    //top
    Rectangle{
        color: "darkblue"
        height: 5
        width: parent.width
        anchors.top:parent.top
    }
    //bottom
    Rectangle{
        color: "darkblue"
        height: 5
        width: parent.width
        anchors.bottom: parent.bottom
    }
    //line 1
    Rectangle{
        color: "darkblue"
        height: parent.height
        width: 5
        anchors.right: parent.right
        anchors.rightMargin: parent.width*0.25
    }
    //line 2
    Rectangle{
        color: "darkblue"
        height: parent.height
        width: 5
        anchors.right: parent.right
        anchors.rightMargin: parent.width*0.5
    }
    //line 3
    Rectangle{
        color: "darkblue"
        height: parent.height
        width: 5
        anchors.right: parent.right
        anchors.rightMargin: parent.width*0.75
    }
    //judge line
    Rectangle{
        color: "green"
        height: 5
        width: parent.width
        anchors.bottom:parent.bottom
        anchors.bottomMargin: parent.height/5
    }

    /*
    MouseArea{
        anchors.fill: parent
        onWheel: {
            if (wheel.modifiers && Qt.ControlModifier) {
                chart.height+=wheel.angleDelta.y / 120;
                if(chart.height>song_slider.value)
                    chart.height=song_slider.value
                else if(chart.height<lane_background.height)
                    chart.height=lane_background.height
            }
        }
    }*/
}
