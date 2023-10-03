import QtQuick 2.15
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Piklema test"

    TableView {
        id: tableView
        anchors.fill: parent

        model: ListModel {
            id: listModel
            ListElement { fieldType: "string"; fieldName: "host"; fieldValue: "test.mosquitto.org"}
            ListElement { fieldType: "number"; fieldName: "port"; fieldValue: "1883" }
			ListElement { fieldType: "string"; fieldName: "username"; fieldValue: "" }
			ListElement { fieldType: "string"; fieldName: "password"; fieldValue: "" }
			ListElement { fieldType: "string"; fieldName: "topic"; fieldValue: "/piklema/test" }
			ListElement { fieldType: "string"; fieldName: "filepath"; fieldValue: "../test.txt" }
        }

        TableViewColumn { role: "fieldType"; title: "FieldType";}
		TableViewColumn { role: "fieldName"; title: "FieldName";}

		TableViewColumn {
			role: "fieldValue"
			title: "Value"

			delegate: TextInput {
				text: model.fieldValue
				onTextChanged: {
					model.fieldValue = text;
				}
			}
		}
    }

    Button {
        id: runButton
        text: "RUN"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
		
		property var mqtt_client;
		objectName: "runButton"
		enabled: mqtt_client ? !mqtt_client.busy : false;
		
		onClicked: {
			mqtt_client.publish_file(
				{
					host: tableView.model.get(0).fieldValue,
					port: tableView.model.get(1).fieldValue,
					username: tableView.model.get(2).fieldValue,
					password: tableView.model.get(3).fieldValue,
					topic: tableView.model.get(4).fieldValue
				},

				tableView.model.get(5).fieldValue);
		}
    }
}
