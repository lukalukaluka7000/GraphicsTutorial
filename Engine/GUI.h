#pragma once
#include<CEGUI/CEGUI.h>
//#include<CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include<CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include<glm/glm.hpp>
#include<string>
#include<SDL/SDL.h>
namespace Engine {
	class GUI
	{
	public:
		void init(const std::string& resourceDirectory);
		void destroy();
		void draw();
		//void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);
		
		/*CEGUI::Window* createWidget(	const std::string& type,
										const glm::vec4& destRectPerc,
										const glm::vec4& destRectPix,
										const std::string& name = "");*/
		//CEGUI::Window* createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name /*= ""*/) {

		void loadScheme(const std::string& schemeFile);
		
		CEGUI::Window* createWidget(const std::string & type, const glm::vec4 & destRectPerc, const glm::vec4 & destRectPix, const std::string & name = "");

		void setFont(const std::string& fontFile);

		void setMouseCursor(const std::string& imageFile);
		void showMouseCursor();
		void hideMouseCursor();

		void OnSDLevent(SDL_Event& evnt);

		CEGUI::Key::Scan SDLKeyToCEGUI(SDL_Keycode /*Uint8*/ keyCode);
		CEGUI::MouseButton SDLMouseToCEGUI(Uint8 mouseTipka);

		static CEGUI::OpenGL3Renderer* getRenderer() { return _renderer; }
		const CEGUI::GUIContext* getContext() { return _context; }

		void update();
	private:
		
		static CEGUI::OpenGL3Renderer* _renderer;
		CEGUI::GUIContext* _context = nullptr;
		CEGUI::Window* _root = nullptr;
		Uint32 _lastTime = SDL_GetTicks();
	};
}
