#include "FlowEditorApp.hpp"
#include <GL3/Window.hpp>
#include <GL3/PerspectiveCamera.hpp>
#include <GL3/Shader.hpp>
#include <Core/Macros.hpp>
#include <glad/glad.h>

FlowEditorApp::FlowEditorApp()
{
	//! Do nothing
}

FlowEditorApp::~FlowEditorApp()
{
	//! Do nothing
}

bool FlowEditorApp::OnInitialize(std::shared_ptr<GL3::Window> window, const cxxopts::ParseResult& configure)
{
	UNUSED_VARIABLE(configure);
	//! Add perspective camera with default settings
	auto defaultCam = std::make_shared<GL3::PerspectiveCamera>();

	if (!defaultCam->SetupUniformBuffer())
		return false;

	defaultCam->SetupCamera(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	defaultCam->SetProperties(window->GetAspectRatio(), 60.0f, 0.001f, 100.0f);
	defaultCam->UpdateMatrix();

	AddCamera(std::move(defaultCam));

	//! Add PBR shader which is main shading pipeline in this application
	auto defaultShader = std::make_shared<GL3::Shader>();
	if (!defaultShader->Initialize({ {GL_VERTEX_SHADER,	  RESOURCES_DIR "shaders/vertex.glsl"},
									 {GL_FRAGMENT_SHADER, RESOURCES_DIR "shaders/output.glsl"} }))
		return false;

	defaultShader->BindUniformBlock("UBOCamera", 0);
	defaultShader->BindUniformBlock("UBOScene", 1);
	_debug.SetObjectName(GL_PROGRAM, defaultShader->GetResourceID(), "Default Program");
	_shaders.emplace("default", std::move(defaultShader));

	return true;
}

void FlowEditorApp::OnCleanUp()
{
	//! Do nothing
}

void FlowEditorApp::OnUpdate(double dt)
{
	UNUSED_VARIABLE(dt);
}

void FlowEditorApp::OnDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.8f, 1.0f);

	//! Do nothing
}

void FlowEditorApp::OnProcessInput(unsigned int key)
{
	UNUSED_VARIABLE(key);
}

void FlowEditorApp::OnProcessResize(int width, int height)
{
	UNUSED_VARIABLE(width);
	UNUSED_VARIABLE(height);
}
