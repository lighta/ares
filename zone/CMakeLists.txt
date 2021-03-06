cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ares_zone LANGUAGES C CXX)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
include(${ARES_DIR}/projects/CMake/shared.cmake)

if (NOT ARES_PACKET_SET)
  message(FATAL_ERROR "ARES_PACKET_SET is not defined. Set it with -DARES_PACKET_SET=your_version command line argument to CMake")
else()
  message("Compiling for packet version ${ARES_PACKET_SET}")
endif()


set(NETWORK "${ARES_DIR}/network")
set(COMMON "${ARES_DIR}/common")
set(MODEL "${ARES_DIR}/model")
set(PACKETS "${ARES_DIR}/packets")
set(DATABASE "${ARES_DIR}/database")
set(CONTRIB "${ARES_DIR}/contrib")
set(DATABASE "${ARES_DIR}/database")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  ${SRC}/map/astar.cpp
  ${SRC}/map/map.cpp
  ${SRC}/map/event_visitor.cpp
  ${SRC}/map/event_pc_walking.cpp
  ${SRC}/pc/pc.cpp
  ${SRC}/maps_manager.cpp
  ${SRC}/pcs_manager.cpp
  ${SRC}/main.cpp
  ${SRC}/server.cpp
  ${SRC}/session.cpp
  ${SRC}/config.cpp
  ${SRC}/auth_request_manager.cpp
  ${SRC}/mono/state.cpp  
  ${SRC}/mono/CZ_ENTER.cpp
  ${SRC}/character_server/state.cpp
  ${SRC}/character_server/ARES_HZ_CHAR_AUTH_RESULT.cpp
  ${SRC}/character_server/ARES_HZ_MAP_IDS.cpp
  ${SRC}/character_server/ARES_HZ_LOGIN_RESULT.cpp
  ${SRC}/character_server/ARES_HZ_PRIVATE_MSG_NAME.cpp
  ${SRC}/character_server/ARES_HZ_PING_ACK.cpp
  ${SRC}/client/state.cpp
  ${SRC}/client/CZ_LESSEFFECT.cpp
  ${SRC}/client/CZ_NOTIFY_ACTORINIT.cpp
  ${SRC}/client/CZ_REQUEST_MOVE.cpp
  ${SRC}/client/CZ_REQ_SCHEDULER_CASHITEM.cpp
  ${SRC}/client/CZ_REQ_GUILD_MENU.cpp
  ${SRC}/client/CZ_REQNAME.cpp
  ${SRC}/client/CZ_REQUEST_TIME.cpp

  )

if (NOT TARGET common) 
  add_subdirectory(${COMMON} common)
endif()

if (NOT TARGET database) 
  add_subdirectory(${DATABASE} database)
endif()

add_subdirectory(${CONTRIB}/lzma_lite_lib lzma)

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${NETWORK}/include
  ${CONFIG}/include
  ${COMMON}/include
  ${MODEL}/include
  ${PACKETS}/include
  ${DATABASE}/include
  ${CONTRIB}/asio/asio/include
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/json/include
  ${CONTRIB}/network_buffer/include
  ${CONTRIB}/RxCpp/Rx/v2/src
  ${CONTRIB}/concurrentqueue
  ${ARES_DIR})

add_executable(ares_zone ${SOURCES})
set_target_properties(ares_zone PROPERTIES
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  C_STANDARD 11
  CXX_STANDARD 17
  )
  
target_link_libraries(ares_zone ${ARES_LINK_LIBRARIES} common database lzma)
target_include_directories(ares_zone PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(ares_zone PRIVATE ${ARES_COMPILE_DEFINITIONS})
