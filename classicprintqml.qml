
import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

import org.maemo.classicprint 1.0

PageStackWindow {
    initialPage: listPage

    showToolBar: !classicPrint.saving

    ClassicPrint {
        id: classicPrint
    }

    Page {
        id: listPage
        property variant menu: undefined
        tools: commonTools

        orientationLock: PageOrientation.LockPortrait

        ListView {
            id: listView

            anchors.fill: parent
            model: fileModel
            spacing: 10

            delegate: Image {
                source: 'file://' + dcimFolder + '/' + modelData
                asynchronous: true
                fillMode: Image.PreserveAspectFit

                height: 200
                width: parent.width

                sourceSize {
                    width: parent.width
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        displayImage.filePath = dcimFolder + '/' + modelData;
                        pageStack.push(imagePage);
                    }
                }

                BusyIndicator {
                    anchors.centerIn: parent
                    running: visible
                    visible: parent.status == Image.Loading
                }
            }
        }

        ScrollDecorator {
            flickableItem: listView
        }
    }

    Page {
        id: imagePage
        tools: commonTools
        orientationLock: PageOrientation.LockPortrait
        property variant menu: imageMenu

        Item {
            anchors.fill: parent
            //contentWidth: displayImage.width
            //contentHeight: displayImage.height
            clip: true

            Image {
                id: displayImage
                visible: !classicPrint.saving
                asynchronous: true
                property string filePath: ''

                //fillMode: Image.PreserveAspectCrop
                anchors.centerIn: parent
                //rotation: 90
                //width: imagePage.height

                //height: imagePage.height
                width: imagePage.width

                source: {
                    if (filePath !== '') {
                        'image://classicPrint/' + filePath + '#' + classicPrint.sequence;
                    } else {
                        ''
                    }
                }

                sourceSize.width: width
                sourceSize.height: height
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    lensPane.state = 'down';
                    filmPane.state = 'down';
                    processingPane.state = 'down';
                }
            }
        }

        Label {
            anchors {
                horizontalCenter: progressBar.horizontalCenter
                bottom: progressBar.top
                bottomMargin: 30
            }
            text: 'Saving photo...'
            visible: classicPrint.saving
        }

        ProgressBar {
            id: progressBar
            anchors.centerIn: parent
            width: parent.width * .9
            visible: classicPrint.working
            value: classicPrint.progress / 100.
        }

        Rectangle {
            id: lensPane
            visible: !classicPrint.saving

            state: 'down'

            states: [
                State {
                    name: 'up'
                    AnchorChanges { target: lensPane; anchors.bottom: imagePage.bottom }
                },
                State {
                    name: 'down'
                    AnchorChanges { target: lensPane; anchors.top: imagePage.bottom }
                }
            ]

            transitions: Transition {
                AnchorAnimation { easing.type: Easing.OutCirc }
            }

            height: lensColumn.height + 2*20
            color: '#a0000000'

            anchors {
                left: parent.left
                right: parent.right
            }

            Column {
                id: lensColumn
                width: parent.width * .8

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 20
                }

                Label { text: 'Radius' }

                Slider {
                    id: radiusSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.radius = value;
                        }
                    }
                }

                Label { text: 'Darkness' }

                Slider {
                    id: darknessSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.darkness = value;
                        }
                    }
                }

                Label { text: 'Dodge' }

                Slider {
                    id: dodgeSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.dodge = value;
                        }
                    }
                }

                Row {
                    width: parent.width
                    spacing: 20

                    Switch {
                        id: defocusSwitch
                        onCheckedChanged: {
                            classicPrint.defocus = checked;
                        }
                    }

                    Label {
                        anchors.verticalCenter: defocusSwitch.verticalCenter
                        text: 'Defocus'
                    }
                }
            }
        }

        Rectangle {
            id: filmPane
            visible: !classicPrint.saving

            state: 'down'

            states: [
                State {
                    name: 'up'
                    AnchorChanges { target: filmPane; anchors.bottom: imagePage.bottom }
                },
                State {
                    name: 'down'
                    AnchorChanges { target: filmPane; anchors.top: imagePage.bottom }
                }
            ]

            transitions: Transition {
                AnchorAnimation { easing.type: Easing.OutCirc }
            }

            height: filmColumn.height + 2*20
            color: '#a0000000'

            anchors {
                left: parent.left
                right: parent.right
            }

            Column {
                id: filmColumn
                width: parent.width * .8

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 20
                }

                Label { text: 'Temperature' }

                Slider {
                    id: temperatureSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.temperature = value;
                        }
                    }
                }

                Label { text: 'Noise' }

                Slider {
                    id: noiseSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.noise = value;
                        }
                    }
                }
            }
        }

        Rectangle {
            id: processingPane
            visible: !classicPrint.saving

            state: 'down'

            states: [
                State {
                    name: 'up'
                    AnchorChanges { target: processingPane; anchors.bottom: imagePage.bottom }
                },
                State {
                    name: 'down'
                    AnchorChanges { target: processingPane; anchors.top: imagePage.bottom }
                }
            ]

            transitions: Transition {
                AnchorAnimation { easing.type: Easing.OutCirc }
            }

            height: processingColumn.height + 2*20
            color: '#a0000000'

            anchors {
                left: parent.left
                right: parent.right
            }

            Column {
                id: processingColumn
                width: parent.width * .8

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 20
                }

                Label { text: 'Contrast' }

                Slider {
                    id: contrastSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.contrast = value;
                        }
                    }
                }

                Label { text: 'Colourisation' }

                Slider {
                    id: colourisationSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.colourisation = value;
                        }
                    }
                }

                Label { text: 'Frame Size' }

                Slider {
                    id: frameSizeSlider
                    width: parent.width
                    onValueChanged: {
                        if (pressed) {
                            classicPrint.frameSize = value;
                        }
                    }
                }

                Label { text: 'Light Leak' }

                ButtonRow {
                    id: lightLeakRow
                    width: parent.width
                    exclusive: true

                    onCheckedButtonChanged: {
                        for (var i=0; i<children.length; i++) {
                            if (checkedButton == children[i]) {
                                classicPrint.lightLeak = i;
                                break;
                            }
                        }
                    }

                    Button { text: 'None' }
                    Button { text: 'Left' }
                    Button { text: 'Right' }
                }
            }
        }
    }

    ContextMenu {
        id: imageMenu

        MenuLayout {
            MenuItem {
                text: 'Save'
                onClicked: classicPrint.save(displayImage.filePath);
            }
        }
    }

    ToolBarLayout {
        id: commonTools

        ToolIcon {
            visible: pageStack.depth > 1
            iconId: 'icon-m-toolbar-back-white'

            onClicked: {
                if (pageStack.depth > 1) {
                    pageStack.pop();
                }
            }
        }

        ToolIcon {
            id: lensTool
            visible: pageStack.currentPage == imagePage
            iconId: 'icon-m-toolbar-image-edit-white'
            anchors {
                right: filmTool.left
                rightMargin: 20
            }
            onClicked: {
                if (lensPane.state === 'up') {
                    lensPane.state = 'down';
                    return;
                }

                radiusSlider.value = classicPrint.radius;
                darknessSlider.value = classicPrint.darkness;
                dodgeSlider.value = classicPrint.dodge;
                defocusSwitch.checked = classicPrint.defocus;

                filmPane.state = 'down';
                processingPane.state = 'down';
                lensPane.state = 'up';
            }
        }

        ToolIcon {
            id: filmTool
            visible: pageStack.currentPage == imagePage
            iconId: 'icon-m-toolbar-all-content-white'
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                if (filmPane.state === 'up') {
                    filmPane.state = 'down';
                    return;
                }

                temperatureSlider.value = classicPrint.temperature;
                noiseSlider.value = classicPrint.noise;

                lensPane.state = 'down';
                processingPane.state = 'down';
                filmPane.state = 'up';
            }
        }

        ToolIcon {
            id: processingTool
            visible: pageStack.currentPage == imagePage
            anchors {
                left: filmTool.right
                leftMargin: 20
            }
            iconId: 'icon-m-toolbar-new-email-white'
            onClicked: {
                if (processingPane.state === 'up') {
                    processingPane.state = 'down';
                    return;
                }

                contrastSlider.value = classicPrint.contrast;
                colourisationSlider.value = classicPrint.colourisation;
                frameSizeSlider.value = classicPrint.frameSize;
                lightLeakRow.checkedButton = lightLeakRow.children[classicPrint.lightLeak];

                lensPane.state = 'down';
                filmPane.state = 'down';
                processingPane.state = 'up';
            }
        }

        ToolIcon {
            visible: pageStack.currentPage.menu !== undefined
            iconId: 'icon-m-toolbar-view-menu-white'

            anchors.right: parent.right

            onClicked: {
                pageStack.currentPage.menu.open();
            }
        }
    }

    Component.onCompleted: {
        theme.inverted = true;
    }
}

