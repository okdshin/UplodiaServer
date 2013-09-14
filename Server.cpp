#ifdef SERVER_UNIT_TEST
#include "Server.h"
#include <iostream>

using namespace uploadia;

int main(int argc, char* argv[])
{
	Server server(54321);
	server.Run();

    return 0;
}

#endif
