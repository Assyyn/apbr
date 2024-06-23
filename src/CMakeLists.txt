target_sources(${apbr}
    PRIVATE main.cpp
            Logger.cpp Logger.hpp
            Colors.hpp
            Shader.cpp Shader.hpp
            ShaderProgram.cpp ShaderProgram.hpp
            Window.cpp Window.hpp
)

target_include_directories(${apbr} PRIVATE "${CMAKE_BINARY_DIR}/config/include" "${CMAKE_BINARY_DIR}/src")