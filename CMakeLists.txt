cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
include(GNUInstallDirs)
project(g3x C)

# CONFIGURE

include_directories("${PROJECT_SOURCE_DIR}/include/")
file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")

include(FindOpenGL)
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIR})
LIST(APPEND libs ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

include(FindGLUT)
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
LIST(APPEND libs ${GLUT_glut_LIBRARY})

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
  LIST(APPEND libs m)
endif(WIN32)

# BUILD

add_library(${PROJECT_NAME} ${sources})
if(BUILD_SHARED_LIBS)
  target_link_libraries(${PROJECT_NAME} ${libs})
endif()

# PKG-CONFIG SUPPORT

set(PC_LIB_NAME ${PROJECT_NAME})
if(WIN32)
  set(PC_LIBS_PRIVATE "-lm -lopengl32 -lglu32 -lfreeglut")
else()
  set(PC_LIBS_PRIVATE "-lm -lGL -lGLU -lGLUT")
endif()
configure_file(${PROJECT_SOURCE_DIR}/g3x.pc.in ${CMAKE_BINARY_DIR}/g3x.pc @ONLY)

# INSTALL

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

file(GLOB headers "${PROJECT_SOURCE_DIR}/include/*.h")
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/g3x.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)
