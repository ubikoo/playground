project(raymarchsphere)

add_executable(${PROJECT_NAME}
    main.cpp
    raymarch.cpp
    common.h
    raymarch.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath coregraphics corecompute)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/core)

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
