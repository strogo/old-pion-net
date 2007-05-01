// -----------------------------------------------------------------
// libpion: a C++ framework for building lightweight HTTP interfaces
// -----------------------------------------------------------------
// Copyright (C) 2007 Atomic Labs, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "HelloModule.hpp"
#include <libpion/Pion.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <signal.h>


using namespace std;
using namespace pion;


/// stops Pion when it receives signals
void handle_signal(int sig)
{
	Pion::stop();
}


int main (int argc, char *argv[])
{
	static const unsigned int DEFAULT_PORT = 8080;

	// parse command line: determine port number
	unsigned int port = DEFAULT_PORT;
	if (argc == 2) {
		port = strtoul(argv[1], 0, 10);
		if (port == 0) port = DEFAULT_PORT;
	} else if (argc != 1) {
		std::cerr << "usage: PionModulesTest [port]" << std::endl;
		return 1;
	}
	
	// setup signal handlers
	signal(SIGINT, handle_signal);

	// initialize log system (use simple configuration)
	PionLogger main_log(PION_GET_LOGGER("Pion"));
	PION_LOG_SETLEVEL_DEBUG(main_log);
	PION_LOG_CONFIG_BASIC;
	
	try {

		// create a server for HTTP & add the Hello module
		HTTPServerPtr http_server(Pion::addHTTPServer(port));
		http_server->addModule(HTTPModulePtr(new HelloModule));
	
		// startup pion
		Pion::start();
	
		// run until stopped
		Pion::join();
		
	} catch (std::exception& e) {
		PION_LOG_FATAL(main_log, "Caught exception in main(): " << e.what());
	}

	return 0;
}

