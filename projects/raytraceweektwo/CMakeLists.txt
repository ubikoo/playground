project(raytraceweektwo)

add_executable(${PROJECT_NAME}
    camera.cpp
    color.cpp
    film.cpp
    isect.cpp
    main.cpp
    material.cpp
    primitive.cpp
    sampler.cpp
    tracer.cpp
    camera.h
    color.h
    common.h
    film.h
    isect.h
    material.h
    primitive.h
    ray.h
    sampler.h
    tracer.h)

target_link_libraries(${PROJECT_NAME} PRIVATE coremath coregraphics)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external)

file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
