#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
	cxxopts::Options options(argv[0], "simple description");

	options.add_options()
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);

	if (result.count("help"))
	{
		std::cout << options.help() << std::endl;
		exit(0);
	}

	return 0;
}