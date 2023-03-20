import QtQuick
import QtQuick.Controls

import ModPacksList


Item {
    enabled: bottomGameMenuEnabled
    Rectangle {
        id: empty_holder_mods
        ComboBox {
            id: selectModpack
            editable: true
            model: ModpacksList {
                list: ObjModpacksContent
            }

            onActivated: loadAndRefresh(currentIndex);
            font.pixelSize: 18
            x: 1075
            y: 790
            width: 300
            height: 30
        }

        Button {
            id: createModpack
            x: 1425
            y: 790
            width: 125
            height: 30
            font.pixelSize: 18
            text: qsTr("Save Modlist")
            onClicked: {
               if(Qt.LeftButton)
                   {
                       if (selectModpack.editText !== "")
                       {
                           ObjModpacksContent.saveModlist(selectModpack.editText)
                           selectModpack.model.refreshList()
                       }
                   }
               }
        }

        Button {
            id: deleteModpack
            x: 1425
            y: 835
            width: 125
            height: 30
            font.pixelSize: 18
            text: qsTr("Delete Modlist")
            onClicked: {
               if(Qt.LeftButton)
                   {
                        qtGeneralBackendObj.removeModpack(selectModpack.currentText)
                        ObjModpacksContent.removeItem(selectModpack.currentIndex)
                   }
               }
        }

        Button {
            id: play
            x: 1575
            y: 880
            width: 125
            font.pixelSize: 20
            text: qsTr("Play")
            onClicked: {
                if(Qt.LeftButton)
                {
                    qtGeneralBackendObj.startGame();
                }
            }
        }
        Button {
            id: importPack
            x: 1575
            y: 790
            width: 125
            height: 30
            font.pixelSize: 20
            text: qsTr("Import")
            onClicked: {
                if(Qt.LeftButton)
                {
                    qtGeneralBackendObj.importPack();
                    objModsList.refreshModlistVector();
                    qmlModsList.refreshModlistTo0();
                }
            }
        }
        Button {
            id: exportPack
            x: 1575
            y: 835
            width: 125
            height: 30
            font.pixelSize: 20
            text: qsTr("Export")
            onClicked: {
                if(Qt.LeftButton)
                {
                    qtGeneralBackendObj.exportPack();
                }
            }
        }
    }

    function loadAndRefresh(index) {
        ObjModpacksContent.loadModlist(index);
        var modlistName = ObjModpacksContent.getModlistName(index);
        selectModpack.editText = modlistName;

        qmlModsList.onEnable();

        qmlModsList.refreshModlistTo0();
    }
    function refreshModpacksList(){
        selectModpack.model.refreshList();
    }
}