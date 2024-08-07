cmake_minimum_required(VERSION 3.5)
project (BasicRT3 VERSION 1.0.0 LANGUAGES CXX )

#=== CREATING COMPILING DATABASE ===#
# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== FINDING PACKAGES ===#cl

# # Locate TinyXml2 package (library)
# # find_package(TinyXml2 REQUIRED)
# # include_directories(${TinyXML2_INCLUDE_DIRS})

# TinyXML2 is not a module package, therefore cannot be found with find_package() command.
# Set those manually, for now.
#set( TinyXML2_INCLUDE_DIRS ${RT3_SOURCE_DIR}"/tinyxml2/" #"/usr/local/include" 
#)
#set( TinyXML2_LIB_DIRS ${RT3_SOURCE_DIR}"/tinyxml2/" #"/usr/local/lib" 
#)
#set( TinyXML2_LIBRARIES "tinyxml2" 
#)

# Set "manually" paths that need to be considered while compiling/linking
include_directories( cameras
                     integrators
                     core
                     ext
                     )
#link_directories( ${TinyXML2_LIB_DIRS} )



#=== SETTING VARIABLES ===#
# Compiling flags
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fexceptions" )
set( RT3_SOURCE_DIR "src" )

#=== main  target ===
add_executable(basic_rt3 ${RT3_SOURCE_DIR}/core/ambientLight.cpp
                         ${RT3_SOURCE_DIR}/core/api.cpp
                         ${RT3_SOURCE_DIR}/core/background.cpp
                         ${RT3_SOURCE_DIR}/core/blinnPhongIntegrator.cpp
                         ${RT3_SOURCE_DIR}/core/blinnPhongMaterial.cpp
                         ${RT3_SOURCE_DIR}/core/camera.cpp
                         ${RT3_SOURCE_DIR}/core/directionalLight.cpp
                         ${RT3_SOURCE_DIR}/core/error.cpp
                         ${RT3_SOURCE_DIR}/core/film.cpp
                         ${RT3_SOURCE_DIR}/core/flatIntegrator.cpp
                         ${RT3_SOURCE_DIR}/core/flatMaterial.cpp
                         ${RT3_SOURCE_DIR}/core/geometric_primitive.cpp
                         ${RT3_SOURCE_DIR}/core/image_io.cpp
                         ${RT3_SOURCE_DIR}/core/integrator.cpp
                         ${RT3_SOURCE_DIR}/core/light.cpp
                         ${RT3_SOURCE_DIR}/core/material.cpp
                         ${RT3_SOURCE_DIR}/core/parser.cpp
                         ${RT3_SOURCE_DIR}/core/pointLight.cpp
                         ${RT3_SOURCE_DIR}/core/primitive.cpp
                         ${RT3_SOURCE_DIR}/core/primitiveAggregate.cpp
                         ${RT3_SOURCE_DIR}/core/scene.cpp
                         ${RT3_SOURCE_DIR}/core/shape.cpp
                         ${RT3_SOURCE_DIR}/core/sphere.cpp
                         ${RT3_SOURCE_DIR}/core/spotLight.cpp
                         ${RT3_SOURCE_DIR}/core/triangle.cpp
                         ${RT3_SOURCE_DIR}/ext/lodepng.cpp
                         ${RT3_SOURCE_DIR}/ext/tiny_obj_loader.cc
                         ${RT3_SOURCE_DIR}/ext/tinyxml2.cpp
                         ${RT3_SOURCE_DIR}/main/rt3.cpp
                        )

#target_link_libraries(basic_rt3 PRIVATE ${TinyXML2_LIBRARIES})

#define C++17 as the standard.
set_property(TARGET basic_rt3 PROPERTY CXX_STANDARD 17)
