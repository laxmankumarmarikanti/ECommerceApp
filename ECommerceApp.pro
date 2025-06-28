QT += core gui widgets uitools
CONFIG += c++17
LIBS += -lpqxx -lpq -lcurl -lbcrypt
SOURCES += \
    src/main.cpp \
    src/services/*.cpp \
    src/windows/*.cpp
HEADERS += \
    src/services/*.h \
    src/windows/*.h
RESOURCES += resources/ECommerceApp.qrc
build_path = $$PWD/build
DESTDIR = $$PWD
OBJECTS_DIR = $$build_path/obj
MOC_DIR = $$build_path/moc
RCC_DIR = $$build_path/rcc
UI_DIR = $$build_path/ui
