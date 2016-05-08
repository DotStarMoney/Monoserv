cmake_minimum_required(VERSION 3.2)
project(Monoserv)

set(EX_PROJ_DIR ${CMAKE_BINARY_DIR}/deps)

configure_file(deps-CMakeLists.txt deps/CMakeLists.txt COPYONLY)
execute_process(COMMAND ${CMAKE_COMMAND} . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps)
execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps)

include_directories(${EX_PROJ_DIR}/INSTALL/include) 
include_directories(${EX_PROJ_DIR}/Source/websocket)
include_directories(${CMAKE_BINARY_DIR}/include)

add_subdirectory(${CMAKE_BINARY_DIR}/../src)
add_subdirectory(${CMAKE_BINARY_DIR}/../include)

set(ABSOLUTE_SRC_FILES "")
set(ABSOLUTE_HDR_FILES "")

foreach(filen ${SRC_FILES})
	list(APPEND ABSOLUTE_SRC_FILES "src/${filen}")
endforeach(filen)

foreach(filen ${HDR_FILES})
	list(APPEND ABSOLUTE_HDR_FILES "include/${filen}")
endforeach(filen)

add_executable(Monoserv ${ABSOLUTE_SRC_FILES} ${ABSOLUTE_HDR_FILES})

target_link_libraries(Monoserv ${EX_PROJ_DIR}/INSTALL/lib/libboost_thread${CMAKE_STATIC_LIBRARY_SUFFIX} dl pthread)
target_link_libraries(Monoserv ${EX_PROJ_DIR}/INSTALL/lib/libboost_system${CMAKE_STATIC_LIBRARY_SUFFIX})
target_link_libraries(Monoserv ${EX_PROJ_DIR}/INSTALL/lib/libboost_chrono${CMAKE_STATIC_LIBRARY_SUFFIX})


get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

