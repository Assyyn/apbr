add_subdirectory(apbr)

target_sources(${apbr}
    PRIVATE main.cpp
)
target_link_libraries(${apbr} PRIVATE apbr-core)
target_include_directories(${apbr} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/apbr/include")