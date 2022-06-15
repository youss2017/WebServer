#include "startup.hpp"
#include "common.hpp"
#include "helper.hpp"
#include <sstream>
#include <fstream>
#include <list>
#include <mutex>
#include <Shlobj.h>
#include "ManageRequest.hpp"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct ConnectionManagmentThread {
	std::list<SOCKET> clients;
};


DWORD WINAPI ConnectionThread(LPVOID param) {
	ConnectionManagmentThread* cmt = (ConnectionManagmentThread*)param;

	while (true) {
		std::vector<std::list<SOCKET>::iterator> disconnectedSockets;
		disconnectedSockets.reserve(cmt->clients.size());
		for (auto client = cmt->clients.begin(); client != cmt->clients.end(); client++) {
			if (WS_ManageRequest(*client) == 0) {
				closesocket(*client);
				disconnectedSockets.push_back(client);
			}
		}
		for (auto& ids : disconnectedSockets)
			cmt->clients.erase(ids);
		//WSAPoll() would this be actually useful in a server application?
		Sleep(2);
	}

	return 0;
}

static int CALLBACK BrowseFolderCallback(
	HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED) {
		LPCTSTR path = reinterpret_cast<LPCTSTR>(lpData);
	}
	return 0;
}


int main(int argc, char** argv) {
	cout << "Welcome to HTTP/1.1 WebServer" << endl;
	cout << "Select Page folder" << endl;

	char choosenDirectory[512];

	BROWSEINFOA b{};
	b.hwndOwner = GetConsoleWindow();
	b.pszDisplayName = choosenDirectory;
	b.lpszTitle = "Choose the folder where the website files are located.";
	b.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
//	SHBrowseForFolderA(&b);

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

	int threadCount = 4;
	ConnectionManagmentThread* pThreads = new ConnectionManagmentThread[threadCount];
	for (int i = 0; i < threadCount; i++) {
		CreateThread(nullptr, 0, ConnectionThread, &pThreads[i], 0, nullptr);
	}

	while (true) {
		SOCKADDR_IN clientAddress;
		SOCKET client = WSAAccept(server, (SOCKADDR*)&clientAddress, nullptr, nullptr, 0);
		if (client == SOCKET_ERROR) {
			continue;
		}

		u_long nonBlocking = 1;
		if (ioctlsocket(client, FIONBIO, &nonBlocking) == SOCKET_ERROR) {
			auto s = WS_GetLastError();
			printf("%s\n", s.c_str());
			closesocket(client);
			continue;
		}

		char szAddrStr[40];
		InetNtopA(AF_INET, &clientAddress.sin_addr, szAddrStr, sizeof(szAddrStr));
		cout << "Client Connected, " << szAddrStr << endl;

		int smallestSocketsCount = INFINITE;
		int chosenThreadIndex = 0;
		for (int i = 0; i < threadCount; i++) {
			if (smallestSocketsCount > pThreads[i].clients.size()) {
				smallestSocketsCount = pThreads[i].clients.size();
				chosenThreadIndex = i;
			}
		}
		pThreads[chosenThreadIndex].clients.push_back(client);
	}

	closesocket(server);
	WS_Exit();
	return 0;
}