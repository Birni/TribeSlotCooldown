cmake_minimum_required (VERSION 3.8)

target_include_directories(${ProjectName}
	PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
)

set( SOURCE_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/Tools.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/IHooks.cpp
)
set( HEADE_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/ICommands.h
   ${CMAKE_CURRENT_SOURCE_DIR}/IHooks.h
   ${CMAKE_CURRENT_SOURCE_DIR}/Requests.h
   ${CMAKE_CURRENT_SOURCE_DIR}/Tools.h
)

target_sources(${ProjectName}
    PUBLIC
		${SOURCE_FILES}
	PUBLIC
		${HEADE_FILES}
)

