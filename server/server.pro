QT += core gui network

TARGET = server
CONFIG += c++11

SOURCES += main.cpp server.cpp
HEADERS += server.h

# OpenCV 头文件路径
INCLUDEPATH += /usr/include/opencv4

# Qt6 头文件路径（通常不需要手动指定，QT += ... 会自动处理）
# INCLUDEPATH += /usr/include/qt6 /usr/include/qt6/QtGui /usr/include/qt6/QtCore

# 链接 OpenCV 库（使用 pkg-config 自动获取，推荐）
CONFIG += link_pkgconfig
PKGCONFIG += opencv4

# 如果 pkg-config 不可用，手动指定 OpenCV 库
# LIBS += -L/usr/lib64 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_face -lopencv_objdetect

# 链接 Qt6 库（QT += ... 已自动处理，通常无需手动指定）
# LIBS += -L/usr/lib64 -lQt6Gui -lQt6Core -lQt6Network
