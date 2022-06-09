#include "helper.hpp"
#include <ctime>

std::string WS_GetCurrentGMTime()
{
	time_t t = time(NULL);
	tm tmp;
	gmtime_s(&tmp, &t);

	char gmtTime[256];

	const char* fmt = "%a, %d %b %y %T GMT";
	strftime(gmtTime, sizeof(gmtTime), fmt, &tmp);

	std::string gmtStr = gmtTime;
	return gmtStr;
}
