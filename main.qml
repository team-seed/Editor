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

    //譜設定
    property double bpm: 0
    property int offset: 0
    property int beat: 0
    property variant notes: []
    property variant part: []
    property int current_part: -1

    //手勢
    property variant gesture: [0,1]
    property variant current_gesture: 0
    //note 類型
    property variant type: [0,1,2]
    property variant current_type: 0
    //swipe 方向
    property variant direc: [0,1,2,3]     //上下左右
    property variant current_direc: 0

    //按鈕高度
    property double button_height: (60/bpm)*1000
    //按鈕數量
    property int button_count :Math.ceil(player.time/((60/bpm)*1000))
    //譜面高度
    property double view_height: (button_height*button_count)-lane.height
    //譜面最上方額外部分
    property int  chart_top_padding: view_height-player.time
    //最下面墊高
    property int  chart_bottom_padding: 0
    //縮放中心點 不能變
    property double chart_center: view_height/2

    //back ground
    Item {
        id:background
        anchors.fill: parent
    }
    //Lane
    Lane{
        id:lane
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
