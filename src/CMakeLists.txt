add_library(RenderToy 
            rtmath.cpp rtmath.h 
            object.cpp object.h
            importer.cpp importer.h 
            world.cpp world.h
            renderer.h renderer.cpp
            exporter.h exporter.cpp
            procedural.h procedural.cpp
            ray.h ray.cpp
            bvh.h bvh.cpp
            material.h material.cpp
            random.h random.cpp
            surfacepoint.h surfacepoint.cpp
            compositor.h compositor.cpp)

set_property(TARGET RenderToy PROPERTY CXX_STANDARD 20)
target_compile_features(RenderToy PRIVATE cxx_std_20)

target_include_directories(RenderToy PUBLIC ../include)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(RenderToy PUBLIC OpenMP::OpenMP_CXX)
endif()
target_compile_options(RenderToy PUBLIC "-march=native")
