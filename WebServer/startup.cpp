#include "startup.hpp"
#include "error.hpp"
#include <iostream>

using namespace std;

bool WS_Startup(int port, SOCKET* pOutSocket)
{
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, 0);

    if (socket == INVALID_SOCKET)
        return false;

    SOCKADDR_IN bindInfo{};
    bindInfo.sin_family = AF_INET;
    bindInfo.sin_addr.S_un.S_addr = INADDR_ANY;
    WSAHtons(socket, (u_short)port, &bindInfo.sin_port);
    if (bind(socket, (SOCKADDR*)&bindInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        cout << "Error While Binding (Port in Use?): " << WS_GetLastError() << endl;
        return false;
    }
    listen(socket, 25);
    *pOutSocket = socket;
    return true;
}

void WS_Exit()
{
    WSACleanup();
}