set(plasmoidplugin_SRCS
    plasmoidplugin.cpp
    systemCalls.cpp
    worker.cpp
    )

add_library(plasmoidplugin SHARED ${plasmoidplugin_SRCS})
find_package(KF5WidgetsAddons)

target_link_libraries(plasmoidplugin Qt5::Quick KF5::Plasma KF5::WidgetsAddons)

install(TARGETS plasmoidplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/private/archUpdate)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/private/archUpdate)
