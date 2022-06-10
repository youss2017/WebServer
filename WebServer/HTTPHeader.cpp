#include "HTTPHeader.hpp"
#include "helper.hpp"

HTTPHeader::HTTPHeader(HTTPStatusCode status)
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

	header << "HTTP/1.1 " << HTTPCodes[(int)status] << "\n" <<
		"Date: " << WS_GetCurrentGMTime() << "\n";
}
