target_sources(${renderer}
    PRIVATE main.cpp
)

target_include_directories(${renderer} PRIVATE "${CMAKE_BINARY_DIR}/config/include")