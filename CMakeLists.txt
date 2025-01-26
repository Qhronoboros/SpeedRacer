cmake_minimum_required(VERSION 3.5.0)
project(SpeedRacer VERSION 0.1.0 LANGUAGES C CXX)

# set(MY_COMPIL_FLAGS ${MY_COMPIL_FLAGS} /fsanitize=address)
# set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "/fsanitize=address")

add_executable(SpeedRacer main.cpp myMathLib.cpp vector2.cpp body.cpp rigidBody.cpp player.cpp car.cpp)

set(SFML_DIR "C:/SFML/SFML-2.6.2-windows-vc17-64-bit/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML 2.6.2 COMPONENTS graphics audio REQUIRED)
target_link_libraries(SpeedRacer sfml-graphics sfml-audio)