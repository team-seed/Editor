import QtQuick 2.0
import QtQuick.Controls 2.12
import Line 1.0
import QtQuick.Layouts 1.12

Rectangle {
    id:lane_background
    property bool first_insert: false
    //property int view_height: 0
    property int contenty: view_height
    //比例 ㄏㄏ
    property double billy: 1
    width:height*0.7
    height:background.height*0.95
    color: "transparent"

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
            contentY: contenty
            model: LineModel{
                id: line_model
                mline: line
            }
            delegate:ButtonLine{
                width:parent.width
                visible: true
            }
            onContentYChanged:{/*
                if(contentY-chart_center>0)
                    song_slider.value = contentY-chart_center
                else if(contentY-chart_center<0)
                    song_slider.value = chart_center-contentY*/
                song_slider.value= ((chart_center+view_height/2)-contentY)/billy
                //console.log(chart_view.contentY)
            }
            Rectangle{
                id: big_pane
                anchors.fill: chart_view
                z:mode
                color: "steelblue"
                MouseArea{
                    anchors.fill:parent
                    onClicked: {
                        console.log("Disable Button Line");
                    }
                }
            }
        }



        Keys.onPressed: {
            if (event.key == Qt.Key_Control) {
                console.log("!")
                mouse.z = 1
                mouse.focus=true
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
        Component.onCompleted: chart_bottom_padding=height
    }

    Button{
        onClicked: {
            console.log(parent.height)
            console.log(view_height)
            console.log(chart_view.contentY)
        }
    }

    //scale lane size
    MouseArea{
        id: mouse
        anchors.fill: sv
        z: -1
        onWheel: {
            chart_center=chart_center
            contenty=contenty
            if (wheel.modifiers){
                if(wheel.angleDelta.y<0){
                   button_height*=0.95
                   if(chart_view.contentY-chart_center>0)
                       chart_view.contentY=chart_center+(chart_view.contentY-chart_center)*0.95
                   else if(chart_view.contentY-chart_center<0)
                       chart_view.contentY=chart_center-(chart_center-chart_view.contentY)*0.95
                   view_height*= 0.95
                   billy*=0.95
                }
                if(wheel.angleDelta.y>0){
                    button_height*=1.05
                    if(chart_view.contentY-chart_center>0)
                        chart_view.contentY=(chart_view.contentY-chart_center)*1.05
                    else if(chart_view.contentY-chart_center<0)
                        chart_view.contentY=(chart_center-chart_view.contentY)*1.05
                    view_height*= 1.05
                    billy*=1.05
                }
            }
        }
        Keys.onReleased: {
             if (event.key == Qt.Key_Control){
                mouse.z = -1
                sv.focus=true
             }
        }
    }

}
