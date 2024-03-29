cmake_minimum_required(VERSION 2.6)
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_C_FLAGS_DEBUG   "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_CXX_FLAGS_DEBUG   "-Wall -Wunknown-pragmas -O0 -g3")
set(CMAKE_BUILD_TYPE Release)

project(libgam)
set(DESIRED_QT_VERSION 4)

#find_package(Qt COMPONENTS QtCore QtGui REQUIRED)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)

set(PACKAGE_SOURCE_CODE
	lib/triangle++/src/assert.cpp
	lib/triangle++/src/del_impl.cpp
	lib/triangle++/src/main.cpp
	src/Gam.cpp
	src/GamGL.cpp
	src/GamJoint.cpp
	src/GamKinect.cpp
	src/GamTriangle.cpp
	include/moc_gam.cxx
)


set(PACKAGE_HEADERS include/gam.hpp)
qt4_wrap_cpp(PACKAGE_HEADERS_MOC ${PACKAGE_HEADERS})

include(${QT_USE_FILE})

set(PACKAGE_EXTRA_LIBRARY
	${QT_QTCORE_LIBRARY}
	${QT_QTWEBKIT_LIBRARY}
	${QT_QTOPENGL_LIBRARY}
	${QT_QTGUI_LIBRARY}
	)


set(PACKAGE_NAME    ${PROJECT_NAME})
set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(PACKAGE_STRING  ${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION})
#set(KONOHA_PACKAGE_DIR konoha/package/${KONOHA_VERSION}/konoha.${PROJECT_NAME})

add_definitions(-DREDUCED -DANSI_DECLARATORS -DTRILIBRARY -DCDT_ONLY)
add_definitions(-DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_BINARY_DIR}
		${KONOHA_INCLUDE_DIRS})
set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/triangle++/include/)

find_path(HAVE_QTGUI QtGui  PATHS ${include_locations})
if(HAVE_QTGUI)
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ${HAVE_QTGUI})
endif()
include_directories(${INCLUDE_DIRS})

find_path(HAVE_QTOPENGL QtOpenGL PATHS ${include_locations})
if(HAVE_QTOPENGL)
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ${HAVE_QTOPENGL})
endif()
include_directories(${INCLUDE_DIRS})

find_path(HAVE_BOX2D Box2D/Box2D.h PATHS ${include_locations})
if(HAVE_BOX2D)
    add_definitions(-DGAM_USING_BOX2D=1)
    include_directories(${HAVE_BOX2D}/Box2D)
endif()

find_path(HAVE_OPENCV opencv/cv.h PATHS ${include_locations})
if(HAVE_OPENCV)
    add_definitions(-DGAM_USING_OPENCV=1)
    include_directories(${HAVE_OPENCV}/opencv)
    include_directories(${HAVE_OPENCV}/opencv2)
endif()

find_path(HAVE_OPENNI ni/XnOpenNI.h PATHS ${include_locations})
if(HAVE_OPENNI)
    add_definitions(-DGAM_USING_OPENNI=1)
    include_directories(${HAVE_OPENNI}/ni)
endif()

find_path(HAVE_NITE nite/XnVNite.h PATHS ${include_locations})
if(HAVE_NITE)
    add_definitions(-DGAM_USING_NITE=1)
    include_directories(${HAVE_NITE}/nite)
endif()

set(NITE_LIBRARIES -lXnVCNITE -lXnVNite)

add_library(${PACKAGE_NAME} SHARED ${PACKAGE_SOURCE_CODE} ${PACKAGE_HEADERS_MOC})

set_target_properties(${PACKAGE_NAME} PROPERTIES PREFIX "")
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
   message(ERROR "OpenGL not found!!")
endif(NOT OPENGL_FOUND)
#MESSAGE(${PACKAGE_EXTRA_LIBRARY})
#MESSAGE(${OPENGL_LIBRARIES})

target_link_libraries(${PACKAGE_NAME} ${PACKAGE_EXTRA_LIBRARY} ${OPENGL_LIBRARIES}
Box2D opencv_calib3d opencv_core opencv_features2d opencv_flann opencv_gpu
opencv_highgui opencv_imgproc opencv_legacy opencv_ml
opencv_objdetect opencv_ts opencv_video OpenNI ${NITE_LIBRARIES})

set(GAM_PACKAGE_DIR gam)
install(TARGETS ${PACKAGE_NAME} DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/gam.hpp DESTINATION include)
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/bin/gamake DESTINATION bin)
