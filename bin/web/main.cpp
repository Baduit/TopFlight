#include <iostream>
#include <sstream>

#include "httplib.h"

#include <Interpreter/Interpreter.hpp>

int main(int argc, char** argv)
{
	httplib::Server server;

	server.Get("/help",
		[&](const httplib::Request&, httplib::Response& res)
		{
			res.set_content("Make a POST request on /execute with your code as a raw text in the body to execute your program", "text/plain");
			res.set_header("Access-Control-Allow-Origin", "*");
		});

	server.Post("/execute",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			std::stringstream program_output;
			TopFlight::Interpreter vm_interpreter(program_output);

			std::stringstream program_input;
			program_input << req.body;
			vm_interpreter.process_stream(program_input);
			res.set_content(program_output.str(), "text/plain");
			res.set_header("Access-Control-Allow-Origin", "*");
		});

	auto mount_point = (argc > 1) ? argv[1] : "./website";
	auto ret = server.set_mount_point("/", mount_point);
	if (!ret)
	{
		std::cout << "Error while mouting ./website as /" << std::endl;
	}

	server.listen("0.0.0.0", 7890);
}