#include "ManageRequest.hpp"
#include "HTTPResponse.hpp"
#include "helper.hpp"
#include <sstream>
#include <fstream>

using namespace std;

const char* pageError404 =
"<!doctype html>\n"
"<!-- Requested resource could not be found -->\n"
"<html lang=\"en-us\">\n"
"<head><title>Error 404</title></head>\n"
"<body>\n"
"<center><h1>404 Not Found</h1><br/><hr/></center>\n"
"</body>\n"
"</html>\n"
;

std::string HeaderBody() {
	stringstream ss;
	ss <<
		"DATE: " << WS_GetCurrentGMTime() << "\n" <<
		"Accept-Encoding: identity\n" <<
		"Connection: Keep-Alive\n";
	return ss.str();
}

const char* get_filename_ext(const char* filename) {
	const char* dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

string HTTPEvaluate(std::string request, int pos) {
	int pos2 = WS_FindFirstIndexOf(request, "/");
	std::string file = st_Trim(request.substr(pos2 + 1, pos - 5));
	cout << "'" << file << "'" << endl;
	auto ReadPageFile = [](const std::string& path, std::string& outContentType, bool& found) throw() -> stringstream {
		std::ifstream io(path);
		stringstream input;
		if (io) {
			const char* ext = get_filename_ext(st_LowerCase(path).c_str());
			outContentType = WS_GetMIMECode(ext);
			input << io.rdbuf();
			found = true;
		}
		else {
			found = false;
			outContentType = "text/html";
			input << pageError404;
		}
		return input;
	};

	if (file.length() == 0) {
		// index file
		file = "index.html";
	}

	string contentType;
	bool found;
	auto input = ReadPageFile(file, contentType, found);

	cout << "Content-Type: " << contentType << endl;

	HTTPResponse response = HTTPResponse(found ? HTTPStatusCode::CODE_200_OK : HTTPStatusCode::CODE_404_NOT_FOUND);
	response.SetLastModified(WS_GetFileLastModified(file));
	response.SetContentType(contentType, false);
	response.SetConnection(true);
	response.SetContentEncoding(contentType);
	response.SetBodyContent(input);

	return response.header.str();
}



int WS_ManageRequest(SOCKET client)
{
	auto SendHTTPMessage = [](SOCKET client, std::stringstream& _msg) {
		std::string msg = _msg.str();
		WSABUF bufDesc;
		bufDesc.len = msg.length();
		bufDesc.buf = (CHAR*)msg.c_str();
		DWORD sendBytes;
		WSASend(client, &bufDesc, 1, &sendBytes, 0, nullptr, nullptr);
	};

	char recvBuf[1024];
	WSABUF recvBufDesc;
	recvBufDesc.len = sizeof(recvBuf);
	recvBufDesc.buf = recvBuf;
	DWORD readBytes;
	DWORD flags = 0;
	if (WSARecv(client, &recvBufDesc, 1, &readBytes, &flags, nullptr, nullptr) == SOCKET_ERROR) {
		return 0;
	}
	recvBuf[readBytes] = '\0';
	printf("%.*s", readBytes, recvBuf);
	// Parse Response
	int pos = WS_FindFirstIndexOf(recvBuf, "HTTP");
	if (pos == -1) {
		HTTPResponse response = HTTPResponse(HTTPStatusCode::CODE_404_NOT_FOUND);
		response.SetConnection(false);
		response.SetContentType("text/html", false);
		const char* page404 = "<!doctype html><html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr/></body></html>";
		response.SetBodyContent(page404, strlen(page404));
		SendHTTPMessage(client, response.header);
	}
	else {
		std::stringstream response;
		response << HTTPEvaluate(recvBuf, pos);
		cout << "[" << response.str() << "]\n";
		SendHTTPMessage(client, response);
	}
	return 1;
}
