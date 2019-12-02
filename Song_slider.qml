import QtQuick 2.0
import QtQuick.Controls 2.2

//slider
Slider {
    id:song_slider
    property double time_position: 0
    wheelEnabled: true
    orientation: Qt.Vertical
    from: 0
    value: time_position
    to: player.time
    implicitWidth: 26
    implicitHeight: parent.height-100
    stepSize: 200
    //release

    onPressedChanged: {
        if(pressed && play_state)
            timer.stop()
        if( !pressed && play_state){
            player.play_from(value)
            timer.restart()
        }
    }

    //移動顯示
    handle: Rectangle {
        x: song_slider.leftPadding + song_slider.availableWidth/2  - width/2
        y: song_slider.bottomPadding + song_slider.visualPosition * (song_slider.availableHeight - height)
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: song_slider.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#e6e6fa"
        Text {
            id: t
            text: {
                Math.floor(song_slider.value/60000)+":"+
                Math.floor(song_slider.value/1000)%60+":"+
                Math.floor(song_slider.value%1000)
            }
        }
    }

    //slider time/val update
    Timer{
        id:timer
        interval: 64
        running: play_state
        repeat: true
        onTriggered: {
            song_slider.value = player.get_position()
        }
    }

    onValueChanged: {
        //譜面連動
        if(lane.contenty != view_height - song_slider.value ){
            lane.contenty = view_height-song_slider.value
        }
        if(song_slider.value>=player.time - 10){
            player.pause()
            play_state = false
        }
    }
}
