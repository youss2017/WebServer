#include "startup.hpp"
#include "common.hpp"
#include "helper.hpp"
#include <sstream>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

std::string HeaderBody() {
	stringstream ss;
	ss <<
		"DATE: " << WS_GetCurrentGMTime() << "\n" <<
		"Accept-Encoding: identity\n" <<
		"Connection: Keep-Alive\n";
	return ss.str();
}

const char* get_filename_ext(const char* filename) {
	const char* dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

stringstream HTTPEvaluate(std::string request, int pos) {
	int pos2 = WS_FindFirstIndexOf(request, "/");
	std::string file = WS_Trim(request.substr(pos2 + 1, pos - 5));
	cout << "'" << file << "'" << endl;

	stringstream response;

	auto ReadPageFile = [](const std::string& path, std::string& outContentType) throw() -> stringstream {
		std::ifstream io("page/" + path);
		stringstream input;
		if (io) {
			const char* ext = get_filename_ext(WS_LowerCase(path).c_str());
			outContentType = WS_GetMIMECode(ext);
			input << io.rdbuf();
		}
		else {
			outContentType = "text/html";
			input << "<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr/></body></html>";
		}
		return input;
	};

	if (file.length() == 0) {
		// index file
		file = "index.html";
	}

	string contentType;
	auto input = ReadPageFile(file, contentType);

	cout << "Content-Type: " << contentType << endl;

	auto input_string = input.str();

	response << "HTTP/1.1 OK 200\n" <<
		HeaderBody() <<
		"Last-Modified: " << WS_GetFileLastModified("page/" + file) << "\n" <<
		"Content-Type: " << contentType << "\n" <<
		"Content-Length: " << input_string.length() << "\n" <<
		"\r\n" <<
		input_string;

	return response;
}

DWORD WINAPI Exp(LPVOID Param) {
	SOCKET client = (SOCKET)Param;

	auto SendHTTPMessage = [client](std::stringstream& _msg){
		std::string msg = _msg.str();
		WSABUF bufDesc;
		bufDesc.len = msg.length();
		bufDesc.buf = (CHAR*)msg.c_str();
		DWORD sendBytes;
		WSASend(client, &bufDesc, 1, &sendBytes, 0, nullptr, nullptr);
	};

	while (true) {
		char recvBuf[1024];
		WSABUF recvBufDesc;
		recvBufDesc.len = sizeof(recvBuf);
		recvBufDesc.buf = recvBuf;
		DWORD readBytes;
		DWORD flags = 0;
		if (WSARecv(client, &recvBufDesc, 1, &readBytes, &flags, nullptr, nullptr) == SOCKET_ERROR) {
			break;
		}
		recvBuf[readBytes] = '\0';
		printf("%.*s", readBytes, recvBuf);
		// Parse Response
		int pos = WS_FindFirstIndexOf(recvBuf, "HTTP");
		if (pos == -1) {
			stringstream response;
			response
				<< "HTTP/1.1 404 Not Found\n" <<
				HeaderBody() <<
				"\r\n";
			SendHTTPMessage(response);
		}
		else {
			std::stringstream response = HTTPEvaluate(recvBuf, pos);
			SendHTTPMessage(response);
		}
	}

	closesocket(client);
	return 0;
}

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
			CreateThread(nullptr, 0, Exp, (LPVOID)client, 0, nullptr);
		}
		else {
			cout << "Client connected with an error." << endl;
		}
		
	}

	closesocket(server);
	WS_Exit();
	return 0;
}