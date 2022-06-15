#pragma once
#include "common.hpp"

/// <summary>
/// Manages client connection
/// </summary>
/// <param name="client"></param>
/// <returns>returns 1 for a connected socket or 0 for a disconnected socket</returns>
int WS_ManageRequest(SOCKET client);
