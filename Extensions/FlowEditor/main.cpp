#include <cxxopts.hpp>
#include <GL3/Window.hpp>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

#include "FlowEditorRenderer.hpp"

int main(int argc, char* argv[])
{
	cxxopts::Options options(argv[0], "simple description");

	options.add_options()
		("t,title", "Window Title(default is 'FlowRenderer')", cxxopts::value<std::string>()->default_value("FlowRenderer"))
		("x,width", "Window width (default is 1200)", cxxopts::value<int>()->default_value("1200"))
		("y,height", "Window height (default is 900)", cxxopts::value<int>()->default_value("900"))
		("h,help", "Print usage");

	auto result = options.parse(argc, argv);

	if (result.count("help"))
	{
		std::cout << options.help() << std::endl;
		exit(0);
	}

	auto renderer = std::make_unique<FlowEditorRenderer>();
	if (!renderer->Initialize(result))
	{
		std::cerr << "Failed to initialize the Renderer" << std::endl;
		return EXIT_FAILURE;
	}
	
	std::shared_ptr< GL3::Window > window = renderer->GetWindow();
	auto startTime = std::chrono::steady_clock::now();
	while (!renderer->GetRendererShouldExit())
	{
		auto nowTime = std::chrono::steady_clock::now();
		double dt = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(nowTime - startTime).count() * 0.001;
		startTime = nowTime;

		renderer->UpdateFrame(dt);
		renderer->DrawFrame();
		
		glfwSwapBuffers(window->GetGLFWWindow());
		glfwPollEvents();
	}

	renderer->CleanUp();

	return 0;
}