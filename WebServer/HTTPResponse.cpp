#include "HTTPResponse.hpp"
#include "helper.hpp"

#define CRLF "\r\n\0"

HTTPResponse::HTTPResponse(HTTPStatusCode status)
{
	// Warning when adding other codes, add them in order
	static const char HTTPCodes[][25] = {
		"100 Continue",
		"101 Switching Protocols",
		"102 Processing",
		"200 OK",
		"201 Created",
		"202 Accepted",
		"204 No Content",
		"205 Reset Content",
		"301 Moved Permanently",
		"302 Found",
		"303 See Other",
		"400 Bad Request",
		"401 Unauthorized",
		"403 Forbidden",
		"404 Not Found"
	};

	header << "HTTP/1.1 " << HTTPCodes[(int)status] << CRLF <<
		"Date: " << WS_GetCurrentGMTime() << CRLF;
}

void HTTPResponse::SetContentType(const std::string& ct_or_ext, bool isExt) {
	header << "Content-Type: ";
	std::string ct;
	if (isExt) {
		header << WS_GetMIMECode(ct_or_ext);
	}
	else {
		header << ct_or_ext;
	}
	header << CRLF;
}

void HTTPResponse::SetLastModified(const std::string& gmtDate) {
	header << "Last-Modified: " << gmtDate << CRLF;
}

void HTTPResponse::SetConnection(bool keepAlive) {
	header << "Connection: " << (keepAlive ? "keep-alive" : "close") << CRLF;
}

void HTTPResponse::SetAcceptEncoding(const std::string& encoding) {
	header << "Accept-Encoding: " << encoding << CRLF;
}

void HTTPResponse::SetAcceptLanguage(const std::string& lang) {
	header << "Accept-Language: " << lang << CRLF;
}

void HTTPResponse::SetContentEncoding(const std::string& encoding) {
	header << "Content-Encoding: " << encoding << CRLF;
}

void HTTPResponse::SetContentLanguage(const std::string& lang) {
	header << "Content-Language: " << lang << CRLF;
}

void HTTPResponse::SetLocation(const std::string& location) {
	header << "Location: " << location << CRLF;
}

void CharToHex(char c, char* pOut /* 2 char wide */, int lowercase) {
	char hex[][31][2] = {
		{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", 0},
		{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", 0}
	};
	if (lowercase) {
		pOut[0] = hex[1][(c >> 4) & 0xf][0];
		pOut[1] = hex[1][c & 0xf][0];
	}
	else {
		pOut[0] = hex[0][(c >> 4) & 0xf][0];
		pOut[1] = hex[0][c & 0xf][0];
	}
}

void HTTPResponse::SetBodyContent(const char* pMessageBody, size_t size)
{
	if (size == 0)
		return;
	
	header << "Content-Length: " << size << CRLF << CRLF;
	header << pMessageBody;
}

void HTTPResponse::SetBodyContent(const std::stringstream& content)
{
	std::string str = content.str();
	header << "Content-Length: " << str.length() << CRLF << CRLF << str;
}
