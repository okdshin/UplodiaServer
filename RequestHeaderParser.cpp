#ifdef REQUESTHEADERPARSER_UNIT_TEST
#include "RequestHeaderParser.h"
#include <iostream>

using namespace uploadia;

int main(int argc, char* argv[])
{
	RequestHeaderParser parser;
	std::string request_str = 
		"GET / HTTP/1.1\r\n"
		"Host: localhost:54321\r\n"
		"\r\n";
	std::vector<uint8_t> request_vect(request_str.begin(), request_str.end());
	RequestHeader request = parser.Parse(request_vect);
	std::cout << request << std::endl;

    return 0;
}

#endif
