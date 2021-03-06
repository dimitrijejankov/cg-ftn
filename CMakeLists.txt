cmake_minimum_required(VERSION 2.8.4)
project(cg_ftn)

# GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

#GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(SOURCE_FILES main.c configuration.c geometry/vector_buffer.c geometry/marching_cubes_data.c geometry/marching_cubes.c utilities/glutil.c utilities/util.c utilities/data_util.c math/mat4.c math/mat3.c math/vec3.c)

add_executable(cg_ftn ${SOURCE_FILES})
target_link_libraries(cg_ftn m ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY})