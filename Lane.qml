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
            spacing: (60/bpm*1000)
            model: LineModel{
                mline: line
            }
            delegate:RowLayout{
                width:parent.width
                Button{
                    id:b1
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B1
                    onClicked:  model.B1 = checked
                    background:Rectangle{
                        height: 5
                        color: b1.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b2
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B2
                    onClicked:  model.B2 = checked
                    background:Rectangle{
                        height: 5
                        color: b2.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b3
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B3
                    onClicked:  model.B3 = checked
                    background:Rectangle{
                        height: 5
                        color: b3.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b4
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B4
                    onClicked:  model.B4 = checked
                    background:Rectangle{
                        height: 5
                        color: b4.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b5
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B5
                    onClicked:  model.B5 = checked
                    background:Rectangle{
                        height: 5
                        color: b5.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b6
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B6
                    onClicked:  model.B6 = checked
                    background:Rectangle{
                        height: 5
                        color: b6.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b7
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B7
                    onClicked:  model.B7 = checked
                    background:Rectangle{
                        height: 5
                        color: b7.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b8
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B8
                    onClicked:  model.B8 = checked
                    background:Rectangle{
                        height: 5
                        color: b8.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b9
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B9
                    onClicked:  model.B9 = checked
                    background:Rectangle{
                        height: 5
                        color: b9.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b10
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B10
                    onClicked:  model.B10 = checked
                    background:Rectangle{
                        height: 5
                        color: b10.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b11
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B11
                    onClicked:  model.B11 = checked
                    background:Rectangle{
                        height: 5
                        color: b11.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b12
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B12
                    onClicked:  model.B12 = checked
                    background:Rectangle{
                        height: 5
                        color: b12.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b13
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B13
                    onClicked:  model.B13 = checked
                    background:Rectangle{
                        height: 5
                        color: b13.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b14
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B14
                    onClicked:  model.B14 = checked
                    background:Rectangle{
                        height: 5
                        color: b14.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b15
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B15
                    onClicked:  model.B15 = checked
                    background:Rectangle{
                        height: 5
                        color: b15.checked ? "red" : "blue"
                    }
                }
                Button{
                    id:b16
                    height: 60
                    Layout.fillWidth: true;
                    checkable: true
                    checked: model.B16
                    onClicked:  model.B16 = checked
                    background:Rectangle{
                        height: 5
                        color: b16.checked ? "red" : "blue"
                    }
                }
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
