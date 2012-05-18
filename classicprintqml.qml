
import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

import org.maemo.classicprint 1.0

PageStackWindow {
    initialPage: listPage

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
                source: 'file://' + modelData
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
                        displayImage.filePath = modelData;
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
                    processingPane.state = 'down';
                }
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: visible
            visible: displayImage.status == Image.Loading
        }

        Rectangle {
            id: processingPane

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
                AnchorAnimation {}
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
            MenuItem { text: 'Lens' }
            MenuItem { text: 'Film' }
            MenuItem {
                text: 'Processing'
                onClicked: {
                    imageMenu.close();
                    contrastSlider.value = classicPrint.contrast;
                    colourisationSlider.value = classicPrint.colourisation;
                    frameSizeSlider.value = classicPrint.frameSize;
                    lightLeakRow.checkedButton = lightLeakRow.children[classicPrint.lightLeak];
                    processingPane.state = 'up';
                }
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

