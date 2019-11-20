import QtQuick 2.0
import QtQuick.Controls 2.2
import Qt.labs.platform 1.1
import QtQuick.Window 2.12
Column {
    id: button_column
    spacing: 10
    width: media_input_button.width
    property variant all_chart: {"BMP_RANGE" :0 ,"SECTION": part}
    property string str_part: ""

    //Open Chart
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id:chart_open
        text:"Open Chart"
        onClicked: openchart.open()

        FileDialog{
            id : openchart
            onAccepted:{
                all_chart=fileIO.openchart(file)
                console.log(all_chart["SECTION"][0]["BEAT"])
            }
        }
    }

    //save Chart
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id:chart_save
        text:"Save Chart"
        onClicked: savechart.open()

        FileDialog{
            id : savechart
            fileMode: FileDialog.SaveFile
            onAccepted:{
                if(current_part != -1)
                    if(!fileIO.savechart(file,all_chart))
                        console.log("save fail")
            }
        }
    }

    //media_input_button
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id: media_input_button
        text: "Select Audio"
        onClicked: openaudio.open()

        FileDialog{
            id: openaudio
            onAccepted: {
                player.set_music(file,1,0)
                song_name.text = fileIO.get_filename(file)
            }
        }
    }

    //play_button (pause)
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id: play_button
        text: play_state ? "Pause" : "Play"
        onClicked: {
            if(player.isready()){
                if(play_state) {
                    player.pause()
                    song_slider.value = player.position
                }
                else {
                    player.play(song_slider.value)
                    start_time = Date.now()
                }
                play_state = !play_state
            }
        }
    }

    //Add section
    Button{
        id: add_section
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Add section"
        onClicked:{
            if(player.isready())
                add_window.show()
        }
        Window{
            id:add_window
            width: 420
            height: 270
            title: "Add section"
            Rectangle{
                id:setting_window_background
                anchors.fill: parent
            }

            Column{
                anchors.verticalCenter: setting_window_background.verticalCenter
                anchors.horizontalCenter: setting_window_background.horizontalCenter
                spacing:20
                //text
                Row{
                    spacing: (add_window.width/4)+5
                    Text {
                        text: "BPM"
                        font.pixelSize: 16
                    }
                    Text {
                        text: "Offset"
                        font.pixelSize: 16
                    }
                    Text {
                        text: "Beat"
                        font.pixelSize: 16
                    }
                }

                //input box
                Row{
                    spacing:add_window.width/10

                    //bpm
                    Rectangle{
                        id: bpm_input_background
                        height: play_button.height
                        width: play_button.width
                        color: "lightgrey"

                        MouseArea{
                            anchors.fill: parent
                            onClicked: bpm_input.focus = true
                        }

                        TextInput{
                            id: bpm_input
                            selectByMouse : true
                            selectionColor : "gray"
                            font.pixelSize : play_button.height * 0.6
                            onAccepted: {
                                text = Number(text)
                                focus = false
                            }
                        }
                    }
                    //offset
                    Rectangle{
                        id: offset_input_background
                        height: play_button.height
                        width: play_button.width
                        color: "lightgrey"
                        MouseArea{
                            anchors.fill:parent
                            onClicked: offset_input.focus = true
                        }
                        TextInput{
                            id: offset_input
                            selectByMouse : true
                            selectionColor : "gray"
                            font.pixelSize : play_button.height * 0.6
                            onAccepted: {
                                text = Number(text)
                                focus = false
                            }
                        }
                    }
                    //beat
                    Rectangle{
                        id: beat_input_background
                        height: play_button.height
                        width: play_button.width
                        color: "lightgrey"

                        MouseArea{
                            anchors.fill:parent
                            onClicked: beat_input.focus = true
                        }

                        TextInput{
                            id: beat_input
                            selectByMouse : true
                            selectionColor : "gray"
                            font.pixelSize : play_button.height * 0.6
                            onAccepted: {
                                text = Number(text)
                                focus = false
                            }
                        }
                    }
                }

                //confirm button
                Row{
                    anchors.horizontalCenter: parent.horizontalCenter
                    //anchors.topMargin: 70
                    Button{
                        text:"Set"
                        onClicked: {
                            if(bpm_input.length!=0 && offset_input.length!=0 && beat_input.length!=0){
                                bpm=bpm_input.text
                                offset=offset_input.text
                                beat=beat_input.text

                                current_part=part.length
                                part[part.length]={"BPM":bpm,"OFFSET":offset,"BEAT":beat,"NOTES":[],}

                                detail.text=("Part : "+(current_part+1)+"\n"+
                                       　    "BPM : "+part[current_part]["BPM"]+"\n"+
                                             "OFFSET : "+part[current_part]["OFFSET"]+"\n"+
                                             "NOTES : "+part[current_part]["NOTES"])

                                bpm_input.text=""
                                offset_input.text=""
                                beat_input.text=""

                                combobox_content.append({"text":"PART"+(current_part+1)})
                                combobox.currentIndex=current_part
                                combobox.a
                                line.setBeatLines(player.time,bpm);
                                add_window.close()
                            }
                        }
                    }
                }
            }
        }
    }

    //part setting
    Text {
        text: "Part Set"
    }
    ComboBox{
        anchors.horizontalCenter: parent.horizontalCenter
        id:combobox
        width: parent.width
        height: play_button.height
        font.pixelSize: 16
        model: ListModel{
            id:combobox_content
        }
        onActivated :{
            current_part=currentIndex
            detail.text=("Part : "+(current_part+1)+"\n"+
                         "BPM : "+part[current_part]["BPM"]+"\n"+
                         "OFFSET : "+part[current_part]["OFFSET"]+"\n"+
                         "NOTES : "+part[current_part]["NOTES"])
        }

    }

    //show detail
    TextArea{
        id:detail
        font.pixelSize: 20
        text:""
    }
}
