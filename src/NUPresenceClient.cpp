// NUPresenceClient.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <vector>
#include <thread>
#include <string>
#include <system_error>

#include <OVR_CAPI.h>

#include <GL/OOGL.hpp>


int createTCPSocket(uint32_t ip, uint16_t port){

	sockaddr_in address;
	memset(&address, 0, sizeof(sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(ip);
	address.sin_port = htons(port);

	// Make our socket
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	try {
		if (fd < 0) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "TCP: We were unable to open the TCP socket");
		}

		// Bind to the 
		if (::connect(fd, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr))) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "TCP: We were unable to bind the TCP socket to the port");
		}
		
	}
	catch (std::system_error e) {
		std::cout <<  e.what() << std::endl;
	}

	return fd;
}

int createUDPSocket(std::string group, uint16_t port, bool multicast){
	// Our multicast group remote
	sockaddr_in address;
	memset(&address, 0, sizeof(sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	// Make our socket
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	if (fd < 0) {
		throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to open the UDP socket");
	}


	if(multicast){
		char yes = true;
		// Set that we reuse the address so more than one application can bind
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to set reuse address on the socket");
		}
	}

	// Bind to the 
	if (::bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr))) {
		throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to bind the UDP socket to the port");
	}
	if(multicast){
		// Our multicast join request
		ip_mreq mreq;
		memset(&mreq, 0, sizeof(mreq));
		mreq.imr_multiaddr.s_addr = inet_addr(group.c_str());
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		// Join our multicast group
		if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<const char*>(&mreq), sizeof(ip_mreq)) < 0) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "There was an error while attemping to join the multicast group");
		}
	}

	return fd;
	
}

void riftThreadMain(){
    bool riftPresent = true;

    GL::Window window(800,600,"Visualisation Window", GL::WindowStyle::Close);
    GL::Context& gl = window.GetContext();

    ovrResult initResult = ovr_Initialize(nullptr);
	if (!OVR_SUCCESS(initResult)) {
		std::cout << "Failed to Init" << std::endl;
		std::cout << initResult << std::endl;
		riftPresent = false;
	}

    ovrHmd HMD;
	ovrGraphicsLuid luid;
    ovrResult result = ovr_Create(&HMD, &luid);
	if (!OVR_SUCCESS(result)) {
		std::cout << "Rift not found!" << std::endl;
		std::cout << result << std::endl;
		riftPresent = false;
	}

    if(riftPresent) ovrHmdDesc hmdDesc = ovr_GetHmdDesc(HMD);
	
	// Main loop
	bool running = true; 
    while (running)
    {
    	if(riftPresent){
	        double           ftiming = ovr_GetPredictedDisplayTime(HMD, 0);
	        // Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
	        ovrTrackingState hmdState = ovr_GetTrackingState(HMD, ftiming, ovrTrue);

			std::cout << "hmdState.HeadPose.ThePose = "
				<< hmdState.HeadPose.ThePose.Orientation.w << " "
				<< hmdState.HeadPose.ThePose.Orientation.x << " "
				<< hmdState.HeadPose.ThePose.Orientation.y << " "
				<< hmdState.HeadPose.ThePose.Orientation.z << " "
				<< hmdState.HeadPose.ThePose.Position.x << " "
				<< hmdState.HeadPose.ThePose.Position.y << " "
				<< hmdState.HeadPose.ThePose.Position.z << " " << std::endl;

    	}
    	
	    GL::Shader vert(GL::ShaderType::Vertex, "#version 150\nin vec2 position; void main() { gl_Position = vec4(position, 0.0, 1.0); }");
	    GL::Shader frag(GL::ShaderType::Fragment, "#version 150\nout vec4 outColor; void main() { outColor = vec4(1.0, 0.0, 0.0, 1.0); }");
	    GL::Program program(vert, frag);

		float vertices[] = {	
	        -0.5f,  0.5f,
	         0.5f,  0.5f,
	         0.5f, -0.5f
	    };
	    GL::VertexBuffer vbo(vertices, sizeof(vertices), GL::BufferUsage::StaticDraw);

	    GL::VertexArray vao;
	    vao.BindAttribute(program.GetAttribute("position"), vbo, GL::Type::Float, 2, 0, 0);

	    GL::Event ev;
	    while (window.IsOpen())
	    {
	        while (window.GetEvent(ev));

	        gl.Clear();

	        gl.DrawArrays(vao, GL::Primitive::Triangles, 0, 3);

	        window.Present();
	    }

    }

    ovr_Destroy(HMD);

}

int main()
{
	std::thread riftThread(riftThreadMain);
	//TODO
	std::string multicastGroup = "239.226.152.162";
	uint16_t multicastPort = 7447;
	std::string myName = "NUPresence Client";

	//SET UP NETWORKING
	WSADATA wsaData;   
	
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		std::cerr << "WSAStartup failed." << std::endl;
		exit(1);
	}
	
	int udpfd = createUDPSocket("0.0.0.0", 0, false); //"0.0.0.0" ~= INADDR_ANY
	int mcudpfd = createUDPSocket(multicastGroup, multicastPort, true);
	
	bool connected = false;
	while (!connected) {
		std::cout << "Waiting for packet.." << std::endl;
		char buff[1500];
		sockaddr_in remote;
		int fromlen = sizeof(sockaddr);
		int len = recvfrom(mcudpfd, buff, sizeof(buff), 0, reinterpret_cast<sockaddr*>(&remote), &fromlen);
		std::cout << "Received data with length = " << len << "Bytes content = " << std::endl;

		if(buff[4] == 1){
			// 9-10 tcp port
			uint16_t tcpPort = *reinterpret_cast<uint16_t*>(buff+9);
			// 11-12 udp port
			uint16_t udpPort = *reinterpret_cast<uint16_t*>(buff+11);
			// string name is null terminated, so just string construct:
			std::string name(buff+13);

			std::cout << "ANNOUNCE: UDP = " << udpPort << " TCP = " << tcpPort << " name = " << name << std::endl;

			if(ntohs(remote.sin_port) == udpPort){
				std::cout << "Ports match. Joining.." << std::endl;

				int tcpfd = createTCPSocket(ntohl(remote.sin_addr.s_addr), tcpPort);

				sockaddr_in addr;
				int len = sizeof(sockaddr_in);
                if (::getsockname(tcpfd, reinterpret_cast<sockaddr*>(&addr), &len) == -1) {
                    throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to get the port from the TCP socket");
                }
                uint16_t myTcpPort = ntohs(addr.sin_port);


				len = sizeof(sockaddr_in);
                if (::getsockname(udpfd, reinterpret_cast<sockaddr*>(&addr), &len) == -1) {
                    throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to get the port from the UDP socket");
                }
                uint16_t myUdpPort = ntohs(addr.sin_port);


				std::vector<char> announcePacket(13 + myName.size() + 1 );
				announcePacket[0] = 0xE2;
				announcePacket[1] = 0x98;
				announcePacket[2] = 0xA2;
				announcePacket[3] = 0x01;
				announcePacket[4] = 1;
				*reinterpret_cast<uint32_t*>(announcePacket.data() + 5) = 4 + myName.size() + 1;
				*reinterpret_cast<uint16_t*>(announcePacket.data() + 9) = myTcpPort;
				*reinterpret_cast<uint16_t*>(announcePacket.data() + 11) = myUdpPort;
				memcpy(announcePacket.data() + 13, myName.c_str(), myName.size() + 1);

				send(tcpfd, announcePacket.data(), announcePacket.size(),0);

				connected = true;
			}
		}

	}

	std::cout << "hello world" << std::endl;
	
	//CLEANUP
	system("pause");
	WSACleanup();
    return 1;
}