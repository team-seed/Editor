import QtQuick 2.0
import QtQuick.Controls 2.12
import Line 1.0
import QtQuick.Layouts 1.12

Rectangle {
    id:lane_background
    property int view_height: 0

    width:height*0.7
    height:background.height*0.95
    color: "gray"
    //chart
    ScrollView{
        id:sv
        anchors.fill:parent
        clip:true
        focus: true
        focusPolicy: Qt.WheelFocus

        ListView{
            id:chart_view
            clip: true
            model: LineModel{
                id:model
                mline: line
            }
            delegate:ButtonLine{
                width:parent.width
                visible: true
            }
        }
    }

    //left
    Rectangle{
        color: "whitesmoke"
        height: parent.height
        width: 5
        anchors.left: parent.left
    }
    //right
    Rectangle{
        color: "whitesmoke"
        height: parent.height
        width: 5
        anchors.right: parent.right
    }
    //top
    Rectangle{
        color: "whitesmoke"
        height: 5
        width: parent.width
        anchors.top:parent.top
    }
    //bottom
    Rectangle{
        color: "whitesmoke"
        height: 5
        width: parent.width
        anchors.bottom: parent.bottom
    }
    //line 1
    Rectangle{
        color: "whitesmoke"
        height: parent.height
        width: 5
        anchors.right: parent.right
        anchors.rightMargin: parent.width*0.25
    }
    //line 2
    Rectangle{
        color: "whitesmoke"
        height: parent.height
        width: 5
        anchors.right: parent.right
        anchors.rightMargin: parent.width*0.5
    }
    //line 3
    Rectangle{
        color: "whitesmoke"
        height: parent.height
        width: 5
        anchors.right: parent.right
        anchors.rightMargin: parent.width*0.75
    }
    //judge line
    Rectangle{
        color: "yellow"
        height: 15
        opacity: 0.5
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
