import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

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
            implicitHeight: (60/bpm*1000)
            color: b1.checked ? "red" : "lightblue"
        }
        Rectangle{
            id:type
            color: (model.type===0)?"black":(model.type===1)?"yellow":"purple"
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
            color: (model.gesture===0)?"royalblue":"orange"
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
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
            implicitHeight: (60/bpm*1000)
            color: b16.checked ? "red" : "lightblue"
        }
    }
}
