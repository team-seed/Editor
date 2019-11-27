import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.12
import QtQuick.Controls.Styles 1.4

RowLayout{
    Button{
        id:b1
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B1
        onClicked:  model.B1 = checked

        //style:ButtonStyle
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b1.checked ? "red" : model.color
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
                    x2:(line.shapeRight(model.previous)+1)*lane_background.width/16 ; y2: line.shapeHeight(model.previous)
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
                PathLine { x:(line.shapeRight(model.previous)+1)*lane_background.width/16; y: line.shapeHeight(model.previous)}
                PathLine { x:(line.shapeLeft(model.previous))*lane_background.width/16 ; y: line.shapeHeight(model.previous)}
                PathLine { x:model.left*lane_background.width/16   ; y: 0}
            }
        }
        /*
        Button{
            id:refresh
            width: 15
            height: 8
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.verticalCenter
            anchors.topMargin: -10
            onClicked: {
                model.refresh = true
                console.log("Model Type: ",model.type)
            }
            background:Rectangle{
                color: "black"
            }
        }*/
        Button{
            id:slicer_2
            width: 15
            height: 8
            visible: model.checkable
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.verticalCenter
            anchors.topMargin: -10
            onClicked: {
                model.refresh = line.sliceAt(model.index,2)        //把index這條Beat切成2等分
            }
            background:Rectangle{
                color: "plum"
            }
        }
        Button{
            id:slicer_3
            width: 15
            height: 8
            visible: model.checkable
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.verticalCenter
            anchors.topMargin: 10
            onClicked: {
                model.refresh = line.sliceAt(model.index,3)        //把index這條Beat切成3等分
            }
            background:Rectangle{
                color: "palegreen"
            }
        }
        Button{
            id:deleter
            width: 15
            height: 15
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            visible: line.deletable(model.index+1)
            onClicked: {
                model.refresh = line.removeLineAt(model.index+1)      //刪掉index+1這條線
            }
            background: Text{
                text: "✖"
                font.pointSize: 10
                color: "red"
            }
        }


        Text{
            text:model.time
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.pointSize: 20
            visible: model.checkable
        }
    }
    Button{
        id:b2
        height: 5
        Layout.fillWidth: true;

        antialiasing: true;
        checkable: model.checkable
        checked: model.B2
        onClicked:  model.B2 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b2.checked ? "red" : model.color
        }
    }
    Button{
        id:b3
        height: 5
        Layout.fillWidth: true;
        checkable: model.checkable
        antialiasing: true;
        checked: model.B3
        onClicked:  model.B3 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b3.checked ? "red" : model.color
        }
    }
    Button{
        id:b4
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B4
        onClicked:  model.B4 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b4.checked ? "red" : model.color
        }
    }
    Button{
        id:b5
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B5
        onClicked:  model.B5 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b5.checked ? "red" : model.color
        }
    }
    Button{
        id:b6
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B6
        onClicked:  model.B6 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b6.checked ? "red" : model.color
        }
    }
    Button{
        id:b7
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B7
        onClicked:  model.B7 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b7.checked ? "red" : model.color
        }
    }
    Button{
        id:b8
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B8
        onClicked:  model.B8 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b8.checked ? "red" : model.color
        }
    }
    Button{
        id:b9
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B9
        onClicked:  model.B9 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b9.checked ? "red" : model.color
        }
    }
    Button{
        id:b10
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B10
        onClicked:  model.B10 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b10.checked ? "red" : model.color
        }
    }
    Button{
        id:b11
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B11
        onClicked:  model.B11 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b11.checked ? "red" : model.color
        }
    }
    Button{
        id:b12
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B12
        onClicked:  model.B12 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b12.checked ? "red" : model.color
        }
    }
    Button{
        id:b13
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B13
        onClicked:  model.B13 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b13.checked ? "red" : model.color
        }
    }
    Button{
        id:b14
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B14
        onClicked:  model.B14 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b14.checked ? "red" : model.color
        }
    }
    Button{
        id:b15
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B15
        onClicked:  model.B15 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b15.checked ? "red" : model.color
        }
    }
    Button{
        id:b16
        height: 5
        Layout.fillWidth: true;
        antialiasing: true;
        checkable: model.checkable
        checked: model.B16
        onClicked:  model.B16 = checked
        background:Rectangle{
            height: model.bold
            implicitHeight: model.button_height
            color: b16.checked ? "red" : model.color
        }
    }
}
