import QtQuick 2.0
import QtQuick.Controls 2.12
import Line 1.0
import QtQuick.Layouts 1.12

Rectangle {
    id:lane_background
    //property int view_height: 0
    property int contenty: player.time+10000
    //比例 ㄏㄏ
    property double billy: 1
    width:height*0.7
    height:background.height*0.95

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
            }
        }
        /*
        Keys.onPressed: {
            if (event.key == Qt.Key_Control) {
                console.log(contenty)
                mouse.z = 1
                mouse.focus=true
            }
        }*/
    }

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
    //scale lane size
    MouseArea{
        id: mouse
        anchors.fill: sv
        z: -1
        onWheel: {
            if (wheel.modifiers){
                if(wheel.angleDelta.y<0)
                   lane_background.contenty*=0.95
                if(wheel.angleDelta.y>0)
                    lane_background.contenty*=1.05
            }
        }
        Keys.onReleased: {
             if (event.key == Qt.Key_Control){
                mouse.z = -1
                sv.focus=true
             }
        }
    }
*/
}
