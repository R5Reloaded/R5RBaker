cmake_minimum_required(VERSION 3.5)

project(R5RBaker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/Qt/6.4.1/msvc2019_64")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp

	src/interfaces/IHasProperties.h
	src/interfaces/IHasActions.h

	src/windows/mainwindow.cpp src/windows/mainwindow.h
	src/windows/cassetlinksdialog.h src/windows/cassetlinksdialog.cpp src/windows/cassetlinksdialog.ui
	src/windows/cnewassetdialog.h src/windows/cnewassetdialog.cpp src/windows/cnewassetdialog.ui
	src/windows/cbuildassetdialog.h src/windows/cbuildassetdialog.cpp src/windows/cbuildassetdialog.ui

        src/panes/cfilesystempane.h src/panes/cfilesystempane.cpp
        src/panes/cdependencypane.h src/panes/cdependencypane.cpp
        src/panes/clogpane.h src/panes/clogpane.cpp
	src/panes/cinspectorpane.h src/panes/cinspectorpane.cpp

	src/editors/ctesteditor.h src/editors/ctesteditor.cpp

        src/logic/assets/cassetgraph.h src/logic/assets/cassetgraph.cpp
        src/logic/assets/casset.h src/logic/assets/casset.cpp

	src/logic/assets/types/crpak.h src/logic/assets/types/crpak.cpp
	src/logic/assets/types/crmdl.h src/logic/assets/types/crmdl.cpp

        src/logic/models/cassetgraphmodel.h src/logic/models/cassetgraphmodel.cpp

        src/views/cdependencytreeview.h src/views/cdependencytreeview.cpp

        src/utils/macros.h
        src/utils/docking.h
	src/utils/structs.h
	src/utils/globals.h

        resources.qrc
)

add_subdirectory(vendor/Qt-Advanced-Docking-System)
add_subdirectory(vendor/tomlplusplus)

qt_add_executable(R5RBaker
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        )

target_link_libraries(R5RBaker PRIVATE qtadvanceddocking)
target_link_libraries(R5RBaker PRIVATE tomlplusplus_tomlplusplus)
target_link_libraries(R5RBaker PRIVATE Qt6::Widgets)
target_link_libraries(R5RBaker PRIVATE d3d11.lib)
target_link_libraries(R5RBaker PRIVATE d3dcompiler.lib)

target_precompile_headers(R5RBaker PRIVATE src/pch.h)

set_target_properties(R5RBaker PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS R5RBaker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_finalize_executable(R5RBaker)
