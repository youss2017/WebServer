#pragma once
#include <sstream>

/*
	The status code is a three-digit integer, and the first digit identifies the general category of response:
	1xx indicates an informational message only
	2xx indicates success of some kind
	3xx redirects the client to another URL
	4xx indicates an error on the client's part
	5xx indicates an error on the server's part
*/
//https://developer.mozilla.org/en-US/docs/Web/HTTP/Status

enum class HTTPStatusCode : unsigned int {
	CODE_100_CONTINUE = 0,
	CODE_101_SWITCH_PROTOCOLS,
	CODE_102_PROCESSSING,
	CODE_200_OK,
	CODE_201_CREATED,
	CODE_202_ACCEPTED,
	CODE_204_NO_CONTENT,
	CODE_205_RESET_CONTENT,
	CODE_301_MOVED_PERMT,
	CODE_302_FOUND,
	CODE_303_SEE_OTHER,
	CODE_400_BAD_REQUEST,
	CODE_401_UNAUTHORIZED,
	CODE_403_FORBIDDEN,
	CODE_404_NOT_FOUND
};

class HTTPResponse {

public:
	HTTPResponse(HTTPStatusCode status);

	void SetContentType(const std::string& ct_or_ext, bool isExt);
	void SetLastModified(const std::string& gmtDate);
	void SetConnection(bool keepAlive);
	void SetAcceptEncoding(const std::string& encoding);
	void SetAcceptLanguage(const std::string& lang = "en-us");
	void SetContentEncoding(const std::string& encoding);
	void SetContentLanguage(const std::string& lang);
	void SetLocation(const std::string& location);

	void SetBodyContent(const char* pMessageBody, size_t size);
	void SetBodyContent(const std::stringstream& content);
		
	std::stringstream header;
};
