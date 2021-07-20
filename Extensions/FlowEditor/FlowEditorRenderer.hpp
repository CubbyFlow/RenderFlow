#ifndef FLOW_EDITOR_RENDERER_HPP
#define FLOW_EDITOR_RENDERER_HPP

#include <GL3/Renderer.hpp>

class FlowEditorRenderer : public GL3::Renderer
{
public:
	//! Default constructor
	FlowEditorRenderer();
	//! Default desctrutor
	~FlowEditorRenderer();
protected:
	bool OnInitialize(const cxxopts::ParseResult& configure) override;
	void OnCleanUp() override;
	void OnUpdateFrame(double dt) override;
	void OnBeginDraw() override;
	void OnEndDraw() override;
	void OnProcessInput(unsigned int key) override;
	void OnProcessResize(int width, int height) override;
};

#endif //! end of FlowEditorRenderer.hpp