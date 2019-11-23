import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.12

RowLayout{
    Button{
        id:b1
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B1
        onClicked:  model.B1 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b1.checked ? "red" : "lightblue"
        }
        Text{
            text:model.direction===0?"↑":model.direction===1?"↓":model.direction===2?"←":"→"
            visible: (model.type!==2)?false:true
            color: "firebrick"
            font.bold: true
            style: Text.Outline; styleColor: "red"
            font.pointSize: 50
            x: (model.left+model.right-1)*lane_background.width/32
            anchors.bottom: parent.top
        }
        Shape{
            anchors.left: parent.left
            anchors.top: parent.top
            visible: (model.type!==1)?false:true
            ShapePath {
               // strokeWidth: 0
                strokeColor: "white"
                fillGradient: LinearGradient {
                    x1:  model.left*lane_background.width/16; y1: 0
                    x2:(line.shapeRight(model.previous)+1)*lane_background.width/16 ; y2: line.shapeHeight(model.previous,bpm,model.button_height)
                    GradientStop { position: 0; color: "lightsalmon" }
                    GradientStop { position: 0.2; color: "tomato" }
                    GradientStop { position: 0.4; color: "red" }
                    GradientStop { position: 0.6; color: "crimson" }
                    GradientStop { position: 1; color: "brown" }
                }
                //strokeStyle: ShapePath.DashLine
                //dashPattern: [ 1, 4 ]
                startX: model.left*lane_background.width/16; startY: 0
                PathLine { x:(model.right+1)*lane_background.width/16; y: 0 }
                PathLine { x:(line.shapeRight(model.previous)+1)*lane_background.width/16; y: line.shapeHeight(model.previous,bpm,model.button_height)}
                PathLine { x:(line.shapeLeft(model.previous))*lane_background.width/16 ; y: line.shapeHeight(model.previous,bpm,model.button_height)}
                PathLine { x:model.left*lane_background.width/16   ; y: 0}
            }
        }
        Rectangle{
            id:type
            color: (model.type===0)?"black":(model.type===1)?"yellow":(model.type===2)?"purple":"white"
            width: 15
            height: 15
            radius: 15
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
        }
        Rectangle{
            id:gesture
            color: (model.gesture===0)?"royalblue":(model.gesture===1)?"orange":"white"
            width: 15
            height: 15
            radius: 15
            anchors.left: type.left
            anchors.leftMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 10
        }
    }
    Button{
        id:b2
        height: 5
        Layout.fillWidth: true;

        antialiasing: true;
        checkable: true
        checked: model.B2
        onClicked:  model.B2 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b2.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b3
        height: 5
        Layout.fillWidth: true;
        checkable: true
        antialiasing: true;
        checked: model.B3
        onClicked:  model.B3 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b3.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b4
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B4
        onClicked:  model.B4 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b4.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b5
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B5
        onClicked:  model.B5 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b5.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b6
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B6
        onClicked:  model.B6 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b6.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b7
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B7
        onClicked:  model.B7 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b7.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b8
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B8
        onClicked:  model.B8 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b8.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b9
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B9
        onClicked:  model.B9 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b9.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b10
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B10
        onClicked:  model.B10 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b10.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b11
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B11
        onClicked:  model.B11 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b11.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b12
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B12
        onClicked:  model.B12 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b12.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b13
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B13
        onClicked:  model.B13 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b13.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b14
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B14
        onClicked:  model.B14 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b14.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b15
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B15
        onClicked:  model.B15 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b15.checked ? "red" : "lightblue"
        }
    }
    Button{
        id:b16
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: true
        checked: model.B16
        onClicked:  model.B16 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b16.checked ? "red" : "lightblue"
        }
    }
}
