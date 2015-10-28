// NUPresenceClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <winsock.h>
#include <vector>

int main()
{
	//TODO
	std::string multicastGroup = "239.226.152.162";
	uint16_t port = 7447;
	uint32_t multicastIF = INADDR_ANY;

	//SET UP NETWORKING
	WSADATA wsaData;   
	
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		std::cerr << "WSAStartup failed." << std::endl;
		exit(1);
	}
 

	// Our multicast group remote
	sockaddr_in address;
	memset(&address, 0, sizeof(sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	// Make our socket
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	try {
		if (fd < 0) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to open the UDP socket");
		}

		char yes = true;
		// Set that we reuse the address so more than one application can bind
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to set reuse address on the socket");
		}

		// Bind to the 
		if (::bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr))) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "We were unable to bind the UDP socket to the port");
		}

		// Our multicast join request
		ip_mreq mreq;
		memset(&mreq, 0, sizeof(mreq));
		mreq.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
		mreq.imr_interface.s_addr = htonl(multicastIF);

		// Join our multicast group
		if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<const char*>(&mreq), sizeof(ip_mreq)) < 0) {
			throw std::system_error(WSAGetLastError(), std::system_category(), "There was an error while attemping to join the multicast group");
		}
	}
	catch (std::system_error e) {
		std::cout <<  e.what() << std::endl;
	}
	
	char buf[1500];
	while (true) {
		sockaddr_in remote;
		int fromlen = sizeof(sockaddr);
		int len = recvfrom(fd, buf, sizeof(buf), 0, reinterpret_cast<sockaddr*>(&remote), &fromlen);
		std::cout << "Received data with length = " << len << "Bytes content = " << std::endl;
	}
	delete[] buf;

	std::cout << "hello world" << std::endl;
	
	//CLEANUP
	system("pause");
	WSACleanup();
    return 1;
}

