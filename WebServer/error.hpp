#pragma once
#include <string>

// Calls WSAGetLastError() and FormatMessage
std::string WS_GetLastError();
