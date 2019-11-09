file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

add_executable(u8086 ${SOURCES} ${HEADERS})

add_dependencies(u8086 gsl)
add_dependencies(u8086 ChaiScript)

find_package(Threads REQUIRED)

set_property(
  TARGET u8086 
  PROPERTY CXX_STANDARD 20
)

target_link_libraries(
  u8086
  ${CMAKE_DL_LIBS}
  Threads::Threads
)

include_directories(
  u8086
  PRIVATE
    "${CMAKE_BINARY_DIR}/include"
)

configure_file(${PROJECT_SOURCE_DIR}/bios.bin ${CMAKE_CURRENT_BINARY_DIR}/bios.bin COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/8086_default.chai ${CMAKE_CURRENT_BINARY_DIR}/8086_default.chai COPYONLY)