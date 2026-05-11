cmake_minimum_required(VERSION 3.31)

project(aizo_proj LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PARAMETERS_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

if(APPLE)
  set(PARAMETERS_LIBRARY_FILE "${PARAMETERS_LIBRARY_DIR}/libparameters.dylib")
  set(PROJECT_RUNTIME_RPATH "@loader_path")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(PARAMETERS_LIBRARY_FILE "${PARAMETERS_LIBRARY_DIR}/libparameters.so")
  set(PROJECT_RUNTIME_RPATH "$ORIGIN")
else()
  message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

if(NOT EXISTS "${PARAMETERS_LIBRARY_FILE}")
  message(FATAL_ERROR "Required shared library not found: ${PARAMETERS_LIBRARY_FILE}")
endif()

add_library(parameters SHARED IMPORTED)
set_target_properties(parameters PROPERTIES
  IMPORTED_LOCATION "${PARAMETERS_LIBRARY_FILE}"
)

add_executable(aizo_proj
  src/main.cpp
)

target_compile_options(aizo_proj PRIVATE
  -Wall
  -Wextra
  -Werror
)

target_include_directories(aizo_proj PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(aizo_proj PRIVATE
  parameters
)

add_custom_command(TARGET aizo_proj POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${PARAMETERS_LIBRARY_FILE}"
          "$<TARGET_FILE_DIR:aizo_proj>"
)

set_target_properties(aizo_proj PROPERTIES
  BUILD_RPATH "${PROJECT_RUNTIME_RPATH}"
  INSTALL_RPATH "${PROJECT_RUNTIME_RPATH}"
)
