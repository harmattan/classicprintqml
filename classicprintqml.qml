
import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1

PageStackWindow {
    initialPage: listPage

    Page {
        id: listPage
        tools: commonTools
        orientationLock: PageOrientation.LockPortrait

        ListView {
            id: listView

            anchors.fill: parent
            model: fileModel

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

        Image {
            id: displayImage
            asynchronous: true
            property string filePath: ''

            width: parent.height
            height: parent.width
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            rotation: 90

            source: 'image://classicPrint/' + filePath
            sourceSize.width: width
            sourceSize.height: height
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: visible
            visible: displayImage.status == Image.Loading
        }
    }

    ToolBarLayout {
        id: commonTools

        ToolIcon {
            iconId: (pageStack.depth>1)?'icon-m-toolbar-back-white':'icon-m-toolbar-back-dimmed-white'
            onClicked: {
                if (pageStack.depth > 1) {
                    pageStack.pop();
                }
            }
        }
    }

    Component.onCompleted: {
        theme.inverted = true;
    }
}

