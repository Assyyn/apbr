add_library(apbr-core STATIC
    Logger.cpp
    Shader.cpp
    ShaderProgram.cpp 
    Window.cpp
    misc.cpp
)

target_link_libraries(apbr-core PRIVATE glfw glad)
target_include_directories(apbr-core PUBLIC "${CMAKE_BINARY_DIR}/config/include" "${CMAKE_CURRENT_SOURCE_DIR}/include")