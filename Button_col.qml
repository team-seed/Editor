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
    property variant temp2:[]
    property variant editorInfo: []
    property variant savedfile: "autoSaved.json"
    property bool autoSaved: false

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
                savedfile = file
                if(all_chart.length===0)
                    console.log("Error")
                else{
                    line.editorFileOpen(all_chart["SECTION"][0]["NOTES"][0])
                    line2.editorFileOpen(all_chart["SECTION"][0]["NOTES"][1])
                }
                part=all_chart["SECTION"]
                bpm=part[0]["BPM"]
                current_part=0
                for(var i=0;i<part.length;i++)
                    select_part_content.append({"text" : "PART"+(i+1)})
                part_select.currentIndex=0
                detail.setdetail()
                view_height = line.getTotalHeight()-lane.height
                autoSaved = true;
            }
        }
    }

    //save Chart
    Timer{
        id: autoSave
        repeat: true
        interval: 6000
        running: autoSaved
        onTriggered: {
            temp = line.editorFileSave()
            temp2 = line2.editorFileSave()
            for(var i=0;i<part.length;i++)
                part[i]["NOTES"] = [];
            part[0]["NOTES"].push([]);
            part[0]["NOTES"].push([]);
            for(var i=0;i<temp.length;i++){
                part[0]["NOTES"][0].push(temp[i]);
            }
            for(var i=0;i<temp2.length;i++){
                part[0]["NOTES"][1].push(temp2[i]);
            }
            all_chart["SECTION"] = part;
            if(savedfile!="autoSaved.json"){
                var index = savedfile.toString().lastIndexOf('/')
                savedfile = savedfile.toString().substring(0,index)+"/autoSaved.json"
            }
            fileIO.savechart(savedfile,all_chart)
            console.log("auto saved")
        }
    }
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id:chart_save
        text:"Save Chart"
        onClicked: {
            if(player.isready()){

                temp = line.editorFileSave()
                temp2 = line2.editorFileSave()
                for(var i=0;i<part.length;i++)
                    part[i]["NOTES"] = [];
                part[0]["NOTES"].push([]);
                part[0]["NOTES"].push([]);
                for(var i=0;i<temp.length;i++){
                    part[0]["NOTES"][0].push(temp[i]);
                }
                for(var i=0;i<temp2.length;i++){
                    part[0]["NOTES"][1].push(temp2[i]);
                }
                all_chart["SECTION"] = part;
                savechart.open()
            }
        }
        FileDialog{
            id : savechart
            fileMode: FileDialog.SaveFile
            onAccepted:{
                savedfile = file
                if(!fileIO.savechart(file,all_chart))
                    console.log("save fail")
            }
        }
    }
    //存成遊戲譜面
    Button{
        anchors.horizontalCenter: parent.horizontalCenter
        id:exp
        text:"Export Chart"
        onClicked: {
            if(player.isready()){

                temp =line.noteOutput();
                temp2 =line2.noteOutput();
                for(var i=0;i<part.length;i++)
                    part[i]["NOTES"] = [];
                for(var i=0;i<temp.length;i++){
                    part[0]["NOTES"][i] = temp[i];
                }
                for(var i=0;i<temp2.length;i++){
                    part[0]["NOTES"].push(temp2[i]);
                }
                all_chart["SECTION"] = part;
                savechart.open()

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
                                line2.setBeatLines(player.time,bpm,beat,offset);
                                view_height = line.getTotalHeight()-lane.height
                                autoSaved = true;
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
            line2.setGesture(gesture_select.currentIndex)
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
            line2.setType(type_select.currentIndex)
            
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
            line2.setDirection(direc_select.currentIndex)
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
        text: "LayerSwitch"
        onCheckStateChanged: {
            mode = mode_switch.checked ? 1 : -1
        }
    }
    Button{
        text:"Hold線完成"
        onClicked: {
            line.holdClear()
            line2.holdClear()
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
                    "BEAT : "+part[current_part]["BEATS"]
        }

    }
}
