cmake_minimum_required(VERSION 3.7)

if(CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
	add_executable(datachannel-p2p main.cpp)
else()
	add_executable(datachannel-p2p main.cpp)
    add_executable(datachannel-p2p-sender sender_peer.cpp)
    add_executable(datachannel-p2p-receiver receiver_peer.cpp) 
endif()

set_target_properties(datachannel-p2p PROPERTIES
    CXX_STANDARD 17
    OUTPUT_NAME p2p)
set_target_properties(datachannel-p2p-sender PROPERTIES
    CXX_STANDARD 17
    OUTPUT_NAME p2p-sender)
set_target_properties(datachannel-p2p-receiver PROPERTIES
    CXX_STANDARD 17
    OUTPUT_NAME p2p-receiver)


find_package(Threads REQUIRED)
target_link_libraries(datachannel-p2p LibDataChannel::LibDataChannel Threads::Threads nlohmann_json::nlohmann_json)
target_link_libraries(datachannel-p2p-sender LibDataChannel::LibDataChannel Threads::Threads nlohmann_json::nlohmann_json)
target_link_libraries(datachannel-p2p-receiver LibDataChannel::LibDataChannel Threads::Threads nlohmann_json::nlohmann_json)