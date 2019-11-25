import QtQuick 2.0
import QtQuick.Controls 2.2
import Qt.labs.platform 1.1
import QtQuick.Window 2.12
Column {
    id: button_column
    spacing: 10
    width: media_input_button.width
    property variant all_chart: {"BPM_RANGE" :0 ,"SECTION": part}
    property string str_part: ""
    property variant temp:[]

    //Open Chart
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id:chart_open
        text:"Open Chart"
        onClicked: {
            if(player.isready())
                openchart.open()
        }
        FileDialog{
            id : openchart
            onAccepted:{
                all_chart=fileIO.openchart(file)
                if(all_chart.length===0)
                    console.log("Error")
                else{
                    if(!line.loadNotes(player.time,all_chart["SECTION"][0]))
                        console.log("歌曲不符合")
                }
                part=all_chart["SECTION"]
                bpm=part[0]["BPM"]
                current_part=0
                for(var i=0;i<part.length;i++)
                    select_part_content.append({"text" : "PART"+(i+1)})
                part_select.currentIndex=0
                detail.setdetail()
            }
        }
    }

    //save Chart
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id:chart_save
        text:"Save Chart"
        onClicked: {
            if(player.isready()){
                temp =line.noteOutput();
                for(var i=0;i<part.length;i++)
                    part[i]["NOTES"] = [];
                for(var i=0;i<temp.length;i++){
                    part[0]["NOTES"][i] = temp[i];
                }
                all_chart["SECTION"] = part;
                savechart.open()
            }
        }
        FileDialog{
            id : savechart
            fileMode: FileDialog.SaveFile
            onAccepted:{
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
                            text: "60"
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
                            text:"0"
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
                            text: "4"
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
                                part[part.length]={"BPM":bpm,"OFFSET":offset,"BEATS":beat,"NOTES":[],}
                                detail.setdetail()

                                bpm_input.text=""
                                offset_input.text=""
                                beat_input.text=""

                                select_part_content.append({"text":"PART"+(current_part+1)})
                                part_select.currentIndex=current_part
                                line.setBeatLines(player.time,bpm,beat,offset);
                                add_window.close()
                            }
                        }
                    }
                }
            }
        }
    }

    //delete section
    Button{
        id:delete_part
        text:"Delete section"
        onClicked:delete_window.show()
        Window{
            id:delete_window
            title:"Section Delete"

            Rectangle{
                id:delete_background
                anchors.fill:parent
            }

            Column{
                ComboBox{
                    id:combobox_delete
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    height: play_button.height
                    font.pixelSize: 16
                    model: select_part_content
                }
                Button{
                    text: "Delete"
                    onClicked: {
                        if(part_select.currentIndex==combobox_delete.currentIndex){
                            part_select.currentIndex=0
                            detail.text=""
                        }//兩個part2
                        for(var i=combobox_delete.currentIndex+1;i<=part_select.currentIndex;i++)
                            select_part_content.setProperty(i,"text","PART"+i)

                        select_part_content.remove(combobox_delete.currentIndex)
                        part.splice(combobox_delete.currentIndex,1)

                        delete_window.close()
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
        id:part_select
        width: parent.width
        height: play_button.height
        font.pixelSize: 16
        model: ListModel{
            id:select_part_content
        }
        onActivated :{
            current_part=currentIndex
            detail.setdetail()
        }
    }

    //手勢
    Text {
        text: "Gesture Set"
    }
    ComboBox{
        anchors.horizontalCenter: parent.horizontalCenter
        id:gesture_select
        width: parent.width
        height: play_button.height
        font.pixelSize: 16
        model: ListModel{
            id:gesture_select_content
        }
        onActivated :{
            current_gesture=gesture_select.currentIndex
            line.setGesture(gesture_select.currentIndex)
        }
        Component.onCompleted: {
            for(var i=0;i<gesture.length;i++)
                gesture_select_content.append({"text":"GES"+i})
        }
    }

    //type
    Text {
        text: "Type Set"
    }
    ComboBox{
        anchors.horizontalCenter: parent.horizontalCenter
        id:type_select
        width: parent.width
        height: play_button.height
        font.pixelSize: 16
        model: ListModel{
            id:type_select_content
        }
        onActivated :{
            current_type=type_select.currentIndex
            line.setType(type_select.currentIndex)
            
        }
        Component.onCompleted: {
            for(var i=0;i<type.length;i++)
                if(i==0)
                    type_select_content.append({"text":"CLICK"})
                else if(i==1)
                    type_select_content.append({"text":"HOLD"})
                else if(i==2)
                    type_select_content.append({"text":"SWIPE"})
        }
    }

    //方向設定
    Text {
        text: "Direc Set"
    }
    ComboBox{
        anchors.horizontalCenter: parent.horizontalCenter
        id:direc_select
        width: parent.width
        height: play_button.height
        font.pixelSize: 16
        model: ListModel{
            id:direc_select_content
        }
        onActivated :{
            current_direc=direc_select.currentIndex
            line.setDirection(direc_select.currentIndex)
        }
        Component.onCompleted: {
            for(var i=0;i<direc.length;i++){
                if(i==0)
                    direc_select_content.append({"text":"Up"})
                else if(i==1)
                    direc_select_content.append({"text":"Down"})
                else if(i==2)
                    direc_select_content.append({"text":"Left"})
                else if(i==3)
                    direc_select_content.append({"text":"Right"})
            }
        }
    }
    CheckBox{
        id:mode_switch
        text: "EditorMode"
        onCheckStateChanged: {
            mode = mode_switch.checked?1:-1
        }
    }

    //show detail
    TextArea{
        id:detail
        font.pixelSize: 20
        text:""
        function setdetail(){
            detail.text="Part : "+(current_part+1)+"\n"+
                    "BPM : "+part[current_part]["BPM"]+"\n"+
                    "OFFSET : "+part[current_part]["OFFSET"]+"\n"+
                    "BEAT : "+part[current_part]["BEAT"]
        }

    }
}
