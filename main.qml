import QtQuick 2.12
import QtQuick.Window 2.12
import FileIO 1.0
import Player 1.0
Window {
    id: main_window
    visible: true
    width: 880
    height: 880
    title: qsTr("Hello World")
    property bool play_state: false
    property double start_time: Date.now()
    property double bpm: 0
    property int offset: 0
    property int beat: 0
    property variant notes: []
    property variant part: []
    property int current_part: -1

    //back ground
    Item {
        id:background
        anchors.fill: parent
    }

    //Lane
    Lane{
        anchors.horizontalCenter: background.horizontalCenter
        anchors.verticalCenter: background.verticalCenter
        anchors.horizontalCenterOffset: -45
    }

    //plugin
    Player{
        id: player
    }
    FileIO{
        id: fileIO
    }

    //col
    Button_col {
        anchors.top:background.top
        anchors.topMargin: 50
        anchors.right: background.right
        anchors.rightMargin: 50
    }

    //song name
    Text{
        id:song_name
        anchors.top:background.top
        anchors.topMargin: 20
        anchors.right: background.right
        anchors.rightMargin: 65
        text:""
    }

    //slider
    Song_slider{
        id: song_slider
        anchors.left: background.left
        anchors.leftMargin: 30
        anchors.verticalCenter: background.verticalCenter
    }

}
