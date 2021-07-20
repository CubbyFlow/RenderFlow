#ifndef FLOW_EDITOR_APP_HPP
#define FLOW_EDITOR_APP_HPP

#include <GL3/Application.hpp>
#include <GL3/Scene.hpp>
#include <GL3/DebugUtils.hpp>
#include <GL3/SkyDome.hpp>

class FlowEditorApp : public GL3::Application
{
public:
	//! Default constructor
	FlowEditorApp();
	//! Default desctrutor
	~FlowEditorApp();
	//! Returns overrided Application title
	const char* GetAppTitle() const override
	{
		return "FlowEditor";
	}
protected:
	bool OnInitialize(std::shared_ptr<GL3::Window> window, const cxxopts::ParseResult& configure) override;
	void OnCleanUp() override;
	void OnUpdate(double dt) override;
	void OnDraw() override;
	void OnProcessInput(unsigned int key) override;
	void OnProcessResize(int width, int height) override;

private:
	GL3::DebugUtils _debug;
};

#endif //! end of FlowEditorApp.hpp