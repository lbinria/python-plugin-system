cmake_minimum_required(VERSION 3.10)
project(SmootherPlugin)

add_library(smoother SHARED smoother.cpp)

target_link_libraries(smoother ${PYTHON_LIBRARIES} ${CMAKE_DL_LIBS} ultimaille)

configure_file(smoother.py smoother.py COPYONLY)