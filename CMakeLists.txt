cmake_minimum_required(VERSION 3.10)
project(MyPythonEmbeddedApp LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

find_package(PythonLibs 3.9 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Get development version of ultimaille directly from git repository 
include(FetchContent)
FetchContent_Declare(
  ultimaille
  GIT_REPOSITORY https://github.com/ssloy/ultimaille
  GIT_TAG        master
)
FetchContent_MakeAvailable(ultimaille)

# Add ultimaille directory to get ultimaille CMakeLists
include_directories(${ultimaille_SOURCE_DIR})
add_subdirectory(plugins/smoother)
add_subdirectory(plugins/test)

# Add imgui
include_directories("imgui")

file(GLOB SOURCES "*.cpp" "*.h" "imgui/*.cpp" "imgui/*.h" "imgui/backends/imgui_impl_glfw.cpp" "imgui/backends/imgui_impl_opengl3.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES} ${CMAKE_DL_LIBS} ultimaille glfw ${OPENGL_LIBRARIES})

