set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/media/nikita/270959b1-e336-43ef-b3bb-b4a90f212bad/SVN/linux/Qt_v5.12.3")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(tracer main.cpp MainWindow.cpp MainWindow.h Tracer.cpp Tracer.h DataProvider.h DataProvider.cpp DataChooser.h DataChooser.cpp TracerModel.cpp TracerModel.h ProcessSelector.cpp ProcessSelector.h)

target_link_libraries(tracer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

set_target_properties(tracer
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
        )