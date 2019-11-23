import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

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
