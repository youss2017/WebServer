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
	CODE_203,
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

class HTTPHeader {

public:
	HTTPHeader(HTTPStatusCode status);

	std::string GetHTTPResponse(char* pMessageBody, size_t size);

private:
	std::stringstream header;
};
