add_executable(tracer main.cpp)

set_target_properties(tracer
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
        )