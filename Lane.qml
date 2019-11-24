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
            contentY: contenty
            model: LineModel{
                id: line_model
                mline: line
            }
            delegate:ButtonLine{
                width:parent.width
                visible: true
            }
            onContentYChanged:{
                if ( (chart_center+view_height/2)-contentY > 0 &&
                        contentY >= (chart_center-view_height/2) )
                    song_slider.value= ((chart_center+view_height/2)-contentY)/billy
            }
        }
/*縮放用
        Keys.onPressed: {
            if (event.key == Qt.Key_Control) {
                console.log("!")
                mouse.z = 1
                mouse.focus=true
            }
        }
*/
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
/*
    //scale lane size
    MouseArea{
        id: mouse
        anchors.fill: sv
        z: -1
        onWheel: {
            chart_center=chart_center
            contenty=contenty
            view_height=view_height

            if (wheel.modifiers){

                if(wheel.angleDelta.y < 0){

                }
                else if(wheel.angleDelta.y > 0){

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
    */
}
/*縮放用
                if(wheel.angleDelta.y<0){
                   billy*=0.95
                   button_height*=0.95
                   if(chart_view.contentY-chart_center>0){

                       console.log("cal: "+ (chart_center+(chart_view.contentY-chart_center)*0.95))
                       chart_view.contentY = chart_center+(chart_view.contentY-chart_center)*0.95
                       console.log("Y after: "+chart_view.contentY)
                       console.log("")

                   }
                   else if(chart_view.contentY-chart_center<0)
                       chart_view.contentY=chart_center-(chart_center-chart_view.contentY)*0.95
                   view_height*= 0.95

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
*/
