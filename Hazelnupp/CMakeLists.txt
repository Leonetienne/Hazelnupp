cmake_minimum_required(VERSION 3.16)
project(Hazelnupp)

###################
# Library project #
###################
set(CMAKE_CXX_STANDARD 17)

FILE(GLOB main_src src/*.cpp)
add_library(${PROJECT_NAME}
  ${main_src}
)

target_include_directories(${PROJECT_NAME} PRIVATE
  include
)

#########
# Tests #
#########
FILE(GLOB test_src test/*.cpp)
add_executable(test
  test/Catch2.h
  ${test_src}
)
target_link_libraries(test ${PROJECT_NAME})

target_include_directories(test PRIVATE
  include
)

##############
# Executable #
##############
FILE(GLOB exec_src exec/*.cpp)
add_executable(exec
  ${exec_src}
)
target_link_libraries(exec ${PROJECT_NAME})

target_include_directories(exec PRIVATE
  include
)
