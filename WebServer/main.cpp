#include "startup.hpp"
#include "common.hpp"
#include "helper.hpp"
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(int argc, char** argv) {

	cout << "Welcome to HTTP/1.1 WebServer" << endl;

	int port = 80;


	if (argc > 1) {
		int customPort = std::atoi(argv[1]);
		if (customPort > 0)
			customPort = port;
	}

	cout << "HTTP:" << port << endl;

	SOCKET server;
	if (!WS_Startup(port, &server)) {
		cout << "Encountered Initalization Error. Could not startup." << endl;
		ExitProcess(1);
	}

	// End Of Line for HTTP/1.1 (expect body)
	const char CLRF[2] = { '\r', '\n' };
	const char HTTPVersion[] = "HTTP/1.1\r\n";

	while (true) {
		SOCKADDR_IN clientAddress;
		SOCKET client = WSAAccept(server, (SOCKADDR*)&clientAddress, nullptr, nullptr, 0);
		if (client != SOCKET_ERROR) {
			char szAddrStr[40];
			InetNtopA(AF_INET, &clientAddress.sin_addr, szAddrStr, sizeof(szAddrStr));
			cout << "Client Connected, " << szAddrStr << endl;
		}
		else {
			cout << "Client connected with an error." << endl;
		}
		WSABUF buf;
		buf.len = 512;
		char rbuf[512];
		buf.buf = rbuf;
		DWORD readBytes;
		DWORD flags = 0;
		WSARecv(client, &buf, 1, &readBytes, &flags, nullptr, nullptr);
		cout << rbuf << endl;

		stringstream sampleResponse;

		sampleResponse <<
			"HTTP/1.1 200 OK\n" <<
			"Content-Type: text/html; charset=utf-8\n" <<
			"Date: " << WS_GetCurrentGMTime() <<
			"Last-Modified: " << WS_GetCurrentGMTime() <<
			"\n\r\n" <<
			"<html><head><title>HellO</title></head><body><center><h1>HI</h1></center><hr/></body></html>\r\n";

		std::string response = sampleResponse.str();

		buf.len = response.size();
		buf.buf = (CHAR*)response.c_str();
		DWORD bytesSend;
		WSASend(client, &buf, 1, &bytesSend, 0, nullptr, nullptr);
		closesocket(client);
	}

	closesocket(server);
	WS_Exit();
	return 0;
}