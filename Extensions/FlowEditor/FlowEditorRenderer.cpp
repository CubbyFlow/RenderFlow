#include <FlowEditorRenderer.hpp>
#include <FlowEditorApp.hpp>
#include <Core/Macros.hpp>
#include <glad/glad.h>

FlowEditorRenderer::FlowEditorRenderer()
{
	//! Do nothing
}

FlowEditorRenderer::~FlowEditorRenderer()
{
	//! Do nothing
}

bool FlowEditorRenderer::OnInitialize(const cxxopts::ParseResult& configure)
{
	if (!AddApplication(std::make_shared<FlowEditorApp>(), configure))
		return false;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

void FlowEditorRenderer::OnCleanUp()
{
	//! Do nothing
}

void FlowEditorRenderer::OnUpdateFrame(double dt)
{
	UNUSED_VARIABLE(dt);
}

void FlowEditorRenderer::OnBeginDraw()
{
	//! Do nothing
}

void FlowEditorRenderer::OnEndDraw()
{
	//! Do nothing
}

void FlowEditorRenderer::OnProcessInput(unsigned int key)
{
	UNUSED_VARIABLE(key);
}

void FlowEditorRenderer::OnProcessResize(int width, int height)
{
	UNUSED_VARIABLE(width);
	UNUSED_VARIABLE(height);
}