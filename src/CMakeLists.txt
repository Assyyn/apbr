target_sources(${renderer}
    PRIVATE main.cpp Logger.cpp Logger.hpp Colors.hpp
)

target_include_directories(${renderer} PRIVATE "${CMAKE_BINARY_DIR}/config/include" "${CMAKE_BINARY_DIR}/src")