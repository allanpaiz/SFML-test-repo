cmake_minimum_required(VERSION 3.16)
project(My_Test_Project LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Find OpenCV package
find_package(OpenCV REQUIRED)

add_executable(my_exe src/main.cpp)

# Add include directory
# target_include_directories(My_Test_Project PRIVATE include)
target_include_directories(my_exe PRIVATE ${OpenCV_INCLUDE_DIRS})

# Add source files
# target_sources(My_Test_Project PRIVATE src/Game.cpp)

# Add assets directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(my_exe PRIVATE sfml-graphics ${OpenCV_LIBS})
target_compile_features(my_exe PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET my_exe
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:my_exe>
        VERBATIM)
endif()

install(TARGETS my_exe)
