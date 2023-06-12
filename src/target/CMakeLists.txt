add_executable(target main.cpp ../tracer/DataChooser.h)

set_target_properties(target
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
        )