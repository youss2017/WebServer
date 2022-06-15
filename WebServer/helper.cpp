#include "helper.hpp"
#include <ctime>
#include <vector>
#include <filesystem>
#include <chrono>
#include <map>
#include <iostream>

static std::map<std::string, std::string> s_ExtToMimeMap;

static std::string WS_GMTFormat(time_t t) {
	tm tmp;
	gmtime_s(&tmp, &t);
	const char* fmt = "%a, %d %b %y %T GMT";
	char gmtTime[256];
	strftime(gmtTime, sizeof(gmtTime), fmt, &tmp);
	std::string result = gmtTime;
	return gmtTime;
}

std::string WS_GetCurrentGMTime()
{
	time_t t = time(NULL);
	return WS_GMTFormat(t);
}

// https://stackoverflow.com/questions/56788745/how-to-convert-stdfilesystemfile-time-type-to-a-string-using-gcc-9
template <typename TP>
static std::time_t to_time_t(TP tp)
{
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
		+ system_clock::now());
	return system_clock::to_time_t(sctp);
}

std::string WS_GetFileLastModified(const std::string& path)
{
	try {
		if (std::filesystem::exists(path)) {
			auto t = to_time_t(
				std::filesystem::last_write_time(path));
			return WS_GMTFormat(t);
		}
		else {
			return WS_GetCurrentGMTime();
		}
	}
	catch (std::exception&) {
		return WS_GetCurrentGMTime();
	}
}

std::string WS_GetMIMECode(const std::string& extension) {
	if (s_ExtToMimeMap.size() == 0) {
		s_ExtToMimeMap.insert({ "aac", "audio/aac" });
		s_ExtToMimeMap.insert({ "abw", "application/x-abiword" });
		s_ExtToMimeMap.insert({ "arc", "application/x-freearc" });
		s_ExtToMimeMap.insert({ "avif", "image/avif" });
		s_ExtToMimeMap.insert({ "avi", "video/x-msvideo" });
		s_ExtToMimeMap.insert({ "azw", "application/vnd.amazon.ebook" });
		s_ExtToMimeMap.insert({ "bin", "application/octet-stream" });
		s_ExtToMimeMap.insert({ "bmp", "image/bmp" });
		s_ExtToMimeMap.insert({ "bz", "application/x-bzip" });
		s_ExtToMimeMap.insert({ "bz2", "application/x-bzip2" });
		s_ExtToMimeMap.insert({ "cda", "application/x-cdf" });
		s_ExtToMimeMap.insert({ "csh", "application/x-csh" });
		s_ExtToMimeMap.insert({ "css", "text/css" });
		s_ExtToMimeMap.insert({ "csv", "text/csv" });
		s_ExtToMimeMap.insert({ "doc", "application/msword" });
		s_ExtToMimeMap.insert({ "docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document" });
		s_ExtToMimeMap.insert({ "eot", "application/vnd.ms-fontobject" });
		s_ExtToMimeMap.insert({ "epub", "application/epub+zip" });
		s_ExtToMimeMap.insert({ "gz", "application/gzip" });
		s_ExtToMimeMap.insert({ "gif", "image/gif" });
		s_ExtToMimeMap.insert({ "htm", "text/html" });
		s_ExtToMimeMap.insert({ "html", "text/html" });
		s_ExtToMimeMap.insert({ "ico", "image/vnd.microsoft.icon" });
		s_ExtToMimeMap.insert({ "ics", "text/calendar" });
		s_ExtToMimeMap.insert({ "jar", "application/java-archive" });
		s_ExtToMimeMap.insert({ "jpeg", "image/jpeg" });
		s_ExtToMimeMap.insert({ "jpg", "image/jpeg" });
		s_ExtToMimeMap.insert({ "js", "text/javascript" });
		s_ExtToMimeMap.insert({ "json", "application/json" });
		s_ExtToMimeMap.insert({ "jsonld", "application/ld+json" });
		s_ExtToMimeMap.insert({ "mid", "audio/x-midi" });
		s_ExtToMimeMap.insert({ "midi", "audio/x-midi" });
		s_ExtToMimeMap.insert({ "mjs", "text/javascript" });
		s_ExtToMimeMap.insert({ "mp3", "audio/mpeg" });
		s_ExtToMimeMap.insert({ "mp4", "video/mp4" });
		s_ExtToMimeMap.insert({ "mpeg", "video/mpeg" });
		s_ExtToMimeMap.insert({ "mpkg", "application/vnd.apple.installer+xml" });
		s_ExtToMimeMap.insert({ "odp", "application/vnd.oasis.opendocument.presentation" });
		s_ExtToMimeMap.insert({ "ods", "application/vnd.oasis.opendocument.spreadsheet" });
		s_ExtToMimeMap.insert({ "odt", "application/vnd.oasis.opendocument.text" });
		s_ExtToMimeMap.insert({ "oga", "audio/ogg" });
		s_ExtToMimeMap.insert({ "ogv", "video/ogg" });
		s_ExtToMimeMap.insert({ "ogx", "application/ogg" });
		s_ExtToMimeMap.insert({ "opus", "audio/opus" });
		s_ExtToMimeMap.insert({ "otf", "font/otf" });
		s_ExtToMimeMap.insert({ "png", "image/png" });
		s_ExtToMimeMap.insert({ "pdf", "application/pdf" });
		s_ExtToMimeMap.insert({ "php", "application/x-httpd-php" });
		s_ExtToMimeMap.insert({ "ppt", "application/vnd.ms-powerpoint" });
		s_ExtToMimeMap.insert({ "pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation" });
		s_ExtToMimeMap.insert({ "rar", "application/vnd.rar" });
		s_ExtToMimeMap.insert({ "rtf", "application/rtf" });
		s_ExtToMimeMap.insert({ "sh", "application/x-sh" });
		s_ExtToMimeMap.insert({ "svg", "image/svg+xml" });
		s_ExtToMimeMap.insert({ "swf", "application/x-shockwave-flash" });
		s_ExtToMimeMap.insert({ "tar", "application/x-tar" });
		s_ExtToMimeMap.insert({ "tif", "image/tiff" });
		s_ExtToMimeMap.insert({ "tiff", "image/tiff" });
		s_ExtToMimeMap.insert({ "ts", "video/mp2t" });
		s_ExtToMimeMap.insert({ "ttf", "font/ttf" });
		s_ExtToMimeMap.insert({ "txt", "text/plain" });
		s_ExtToMimeMap.insert({ "vsd", "application/vnd.visio" });
		s_ExtToMimeMap.insert({ "wav", "audio/wav" });
		s_ExtToMimeMap.insert({ "weba", "audio/webm" });
		s_ExtToMimeMap.insert({ "webm", "video/webm" });
		s_ExtToMimeMap.insert({ "webp", "image/webp" });
		s_ExtToMimeMap.insert({ "woff", "font/woff" });
		s_ExtToMimeMap.insert({ "woff2", "font/woff2" });
		s_ExtToMimeMap.insert({ "xhtml", "application/xhtml+xml" });
		s_ExtToMimeMap.insert({ "xls", "application/vnd.ms-excel" });
		s_ExtToMimeMap.insert({ "xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" });
		s_ExtToMimeMap.insert({ "xml", "application/xml, text/xml " });
		s_ExtToMimeMap.insert({ "xul", "application/vnd.mozilla.xul+xml" });
		s_ExtToMimeMap.insert({ "zip", "application/zip" });
		s_ExtToMimeMap.insert({ "3gp", "video/3gpp" });
		s_ExtToMimeMap.insert({ "3g2", "video/3gpp2" });
		s_ExtToMimeMap.insert({ "7z", "application/x-7z-compressed" });
	}
	std::string ext = st_ReplaceAll(st_LowerCase(extension), " ", "");
	std::cout << "'" << ext << "' --> " << s_ExtToMimeMap[ext] << "\n";
	return s_ExtToMimeMap[ext];
}

#pragma region STRING_UTILS

// returns -1 if could not find str
int  WS_FindFirstIndexOf(const std::string& line, const char* str)
{
	int strPos = 0;
	int linePos = 0;
	std::string temp = "";
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == str[strPos])
		{
			if (strPos == 0)
				linePos = i;
			strPos++;
			temp += line[i];
			if (strcmp(temp.c_str(), str) == 0)
			{
				return linePos;
			}
		}
		else
		{
			strPos = 0;
			temp = "";
		}
	}
	return -1;
}

std::string  WS_RemoveAll(const std::string& LINE, const char* str)
{
	size_t length = strlen(str);
	int pos = -1;
	std::vector<int> strPositions;
	std::string temp = LINE;
	while ((pos = WS_FindFirstIndexOf(temp, str)) != -1)
	{
		strPositions.push_back(pos);
		temp = temp.substr(pos + length);
	}
	if (strPositions.size() == 0)
		return LINE;

	std::string result = "";
	int lastPosition = 0;
	for (auto position : strPositions)
	{
		result += LINE.substr(lastPosition, position);
		lastPosition += position + length;
	}
	return result;
}

int  WS_GetFirstNumber(const std::string& line, bool& fail)
{
	auto isNumber = [](char c) throw()->int
	{
		char numbersList[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		int i = 0;
		while (i < 10)
		{
			if (c == numbersList[i])
				return i;
			i++;
		}
		return -1;
	};
	size_t stringIndex = 0;
	int result = 0;
	bool IsNegative = false;
	fail = true;
	while (stringIndex < line.size())
	{
		int v = isNumber(line[stringIndex]);
		if (v != -1)
		{
			result *= 10;
			result += v;
			fail = false;
		}
		else if (line[stringIndex] == '-')
		{
			if (IsNegative)
				break;
			else
				IsNegative = true;
		}
		else
		{
			if (fail == false)
				break;
		}
		stringIndex++;
	}
	if (IsNegative)
		result *= -1;
	return result;
}

bool  WS_EqualNotCaseSensitive(const std::string& a, const std::string& b)
{
	std::string _a = st_LowerCase(a);
	std::string _b = st_LowerCase(b);
	return _a.compare(_b) == 0;
}

bool  WS_EqualNotCaseSensitive(const std::string& a, const char* b)
{
	std::string _a = st_LowerCase(a);
	size_t size = strlen(b);
	char* _cpy = (char*)alloca(size + 1);
	for (size_t i = 0; i < size; i++)
		_cpy[i] = tolower(b[i]);
	_cpy[size] = '\0';
	return _a.compare(_cpy) == 0;
}
#pragma endregion STRING_UTILS
// STRING UTILS END