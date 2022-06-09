#include "error.hpp"
#include <Windows.h>
#include <WinSock2.h>

std::string WS_GetLastError()
{
    int errorCode = WSAGetLastError();

    char* s = NULL;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&s, 0, NULL);
    fprintf(stderr, "%S\n", s);

    std::string errorMessage = s;

    LocalFree(s);

    return errorMessage;
}
