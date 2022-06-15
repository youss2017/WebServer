#pragma once
#include <string>
#include "../StringTools/stringtool.hpp"

std::string WS_GetCurrentGMTime();
std::string WS_GetFileLastModified(const std::string& path);
std::string WS_GetMIMECode(const std::string& extension);

// returns -1 if could not find str
int WS_FindFirstIndexOf(const std::string& line, const char* str);
std::string WS_RemoveAll(const std::string& LINE, const char* str);
bool WS_EqualNotCaseSensitive(const std::string& a, const std::string& b);
bool WS_EqualNotCaseSensitive(const std::string& a, const char* b);

// fail is set to true if no number could be found
// doesn't work with negative numbers (returns the number without the sign)
int WS_GetFirstNumber(const std::string& line, bool& fail);