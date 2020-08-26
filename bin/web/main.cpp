#include <iostream>
#include <sstream>

#include "external/httplib.h"

#include <Interpreter/Interpreter.hpp>

int main()
{
	std::stringstream program_output;
	TopFlight::Interpreter vm_interpreter(program_output);

	httplib::Server server;

	server.Get("/",
		[&](const httplib::Request&, httplib::Response& res)
		{
			res.set_content("Make a POST request on /execute with your code as a raw text in the body to execute your program", "text/plain");
			res.set_header("Access-Control-Allow-Origin", "*");
		});

	server.Post("/execute",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			std::stringstream ss;
			ss << req.body;
			vm_interpreter.process_stream(ss);
			res.set_content(program_output.str(), "text/plain");
			res.set_header("Access-Control-Allow-Origin", "*");
			program_output.str("");
		});

	server.listen("localhost", 7890);
}