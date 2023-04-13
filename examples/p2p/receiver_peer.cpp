
#include "rtc/rtc.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#include <nlohmann/json.hpp>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
typedef int SOCKET;
#endif

using nlohmann::json;

const int BUFFER_SIZE = 2048;

int main() {
    try{
		rtc::InitLogger(rtc::LogLevel::Debug);
		auto pc = std::make_shared<rtc::PeerConnection>();

		pc->onStateChange(
		    [](rtc::PeerConnection::State state) { std::cout << "State: " << state << std::endl; });

		pc->onGatheringStateChange([pc](rtc::PeerConnection::GatheringState state) {
			std::cout << "Gathering State: " << state << std::endl;
			if (state == rtc::PeerConnection::GatheringState::Complete) {
				auto description = pc->localDescription();
				json message = {{"type", description->typeString()},
				                {"sdp", std::string(description.value())}};
				std::cout << message << std::endl;
			}
		});

        SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(6000);

		

        std::cout << "Expect RTP video traffic on localhost:5000" << std::endl;
		std::cout << "Please copy/paste the offer provided by other peer: " << std::endl;
		std::string sdp;
		std::getline(std::cin, sdp);

		std::cout << "Got offer" << sdp << std::endl;
		json j = json::parse(sdp);
		rtc::Description offer(j["sdp"].get<std::string>(), j["type"].get<std::string>());
		pc->setRemoteDescription(offer);

        rtc::Description::Video media("video", rtc::Description::Direction::RecvOnly);
		media.addH264Codec(96);
		media.setBitrate(
		    3000); // Request 3Mbps (Browsers do not encode more than 2.5MBps from a webcam)
		auto track = pc->addTrack(media);
        
        pc->setLocalDescription();

		


    }
    catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}    
}