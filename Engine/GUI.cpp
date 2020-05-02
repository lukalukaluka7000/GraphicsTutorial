#include<GL/glew.h>
#include "GUI.h"
#include<SDL/SDL.h>

namespace Engine {
	//CEGUI::OpenGLRenderer& myRenderer = CEGUI::OpenGLRenderer::bootstrapSystem();
	CEGUI::OpenGL3Renderer* GUI::_renderer = nullptr;

	void GUI::init(const std::string& resourceDirectory) {
		if (_renderer == nullptr) {
			//CEGUI::OpenGLRenderer& myRenderer = CEGUI::OpenGLRenderer::bootstrapSystem();
			_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
		}
		//provide resources
		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
			(CEGUI::System::getSingleton().getResourceProvider());
		rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
		rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
		rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
		rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/"); // window manager
		rp->setResourceGroupDirectory("looknfeel", resourceDirectory + "/looknfeel/"); // widgets
		rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

		//create a context
		_context = &CEGUI::System::getSingleton().createGUIContext(_renderer->getDefaultRenderTarget());

		_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
		_context->setRootWindow(_root);

	}
	void GUI::destroy() {
		CEGUI::System::getSingleton().destroyGUIContext(*_context);
	}
	void GUI::draw() {
		glDisable(GL_DEPTH_TEST);
		_renderer->beginRendering();

		_context->draw();

		_renderer->endRendering();

		glDisable(GL_SCISSOR_TEST);

		// Clean up after CEGUI , seems like this is very important to me
		glBindVertexArray(0);
		glDisable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	void GUI::setFont(const std::string& fontFile) {
		CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
		_context->setDefaultFont(fontFile);
	}
	CEGUI::Window* GUI::createWidget(const std::string& type,
		const glm::vec4& destRectPerc,
		const glm::vec4& destRectPix,
		const std::string& name) {
		CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);;
		//newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);

		_root->addChild(newWindow);

		//newWindow->setPosition(cosnt Uvector2)
		//newWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
		//newWindow->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
		newWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
		newWindow->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w)));
		return newWindow;
	}
	void GUI::loadScheme(const std::string& schemeFile) {
		CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
	}
	void GUI::setMouseCursor(const std::string& imageFile) {
		_context->getMouseCursor().setDefaultImage(imageFile);
	}
	void GUI::showMouseCursor() {
		_context->getMouseCursor().show();
	}
	void GUI::hideMouseCursor() {
		_context->getMouseCursor().hide();
	}
	CEGUI::Key::Scan GUI::SDLKeyToCEGUI(SDL_Keycode /*Uint8*/ keyCode) {
        using namespace CEGUI;
        switch (keyCode)
        {
        case SDLK_BACKSPACE:    return CEGUI::Key::Backspace;
        case SDLK_TAB:          return CEGUI::Key::Tab;
        case SDLK_RETURN:       return CEGUI::Key::Return;
        case SDLK_PAUSE:        return CEGUI::Key::Pause;
        case SDLK_ESCAPE:       return CEGUI::Key::Escape;
        case SDLK_SPACE:        return CEGUI::Key::Space;
        case SDLK_COMMA:        return CEGUI::Key::Comma;
        case SDLK_MINUS:        return CEGUI::Key::Minus;
        case SDLK_PERIOD:       return CEGUI::Key::Period;
        case SDLK_SLASH:        return CEGUI::Key::Slash;
        case SDLK_0:            return CEGUI::Key::Zero;
        case SDLK_1:            return CEGUI::Key::One;
        case SDLK_2:            return CEGUI::Key::Two;
        case SDLK_3:            return CEGUI::Key::Three;
        case SDLK_4:            return CEGUI::Key::Four;
        case SDLK_5:            return CEGUI::Key::Five;
        case SDLK_6:            return CEGUI::Key::Six;
        case SDLK_7:            return CEGUI::Key::Seven;
        case SDLK_8:            return CEGUI::Key::Eight;
        case SDLK_9:            return CEGUI::Key::Nine;
        case SDLK_COLON:        return CEGUI::Key::Colon;
        case SDLK_SEMICOLON:    return CEGUI::Key::Semicolon;
        case SDLK_EQUALS:       return CEGUI::Key::Equals;
        case SDLK_LEFTBRACKET:  return CEGUI::Key::LeftBracket;
        case SDLK_BACKSLASH:    return CEGUI::Key::Backslash;
        case SDLK_RIGHTBRACKET: return CEGUI::Key::RightBracket;
        case SDLK_a:            return CEGUI::Key::A;
        case SDLK_b:            return CEGUI::Key::B;
        case SDLK_c:            return CEGUI::Key::C;
        case SDLK_d:            return CEGUI::Key::D;
        case SDLK_e:            return CEGUI::Key::E;
        case SDLK_f:            return CEGUI::Key::F;
        case SDLK_g:            return CEGUI::Key::G;
        case SDLK_h:            return CEGUI::Key::H;
        case SDLK_i:            return CEGUI::Key::I;
        case SDLK_j:            return Key::J;
        case SDLK_k:            return Key::K;
        case SDLK_l:            return Key::L;
        case SDLK_m:            return Key::M;
        case SDLK_n:            return Key::N;
        case SDLK_o:            return Key::O;
        case SDLK_p:            return Key::P;
        case SDLK_q:            return Key::Q;
        case SDLK_r:            return Key::R;
        case SDLK_s:            return Key::S;
        case SDLK_t:            return Key::T;
        case SDLK_u:            return Key::U;
        case SDLK_v:            return Key::V;
        case SDLK_w:            return Key::W;
        case SDLK_x:            return Key::X;
        case SDLK_y:            return Key::Y;
        case SDLK_z:            return Key::Z;
        case SDLK_DELETE:       return Key::Delete;
      
        case SDLK_KP_PERIOD:    return Key::Decimal;
        case SDLK_KP_DIVIDE:    return Key::Divide;
        case SDLK_KP_MULTIPLY:  return Key::Multiply;
        case SDLK_KP_MINUS:     return Key::Subtract;
        case SDLK_KP_PLUS:      return Key::Add;
        case SDLK_KP_ENTER:     return Key::NumpadEnter;
        case SDLK_KP_EQUALS:    return Key::NumpadEquals;
        case SDLK_UP:           return Key::ArrowUp;
        case SDLK_DOWN:         return Key::ArrowDown;
        case SDLK_RIGHT:        return Key::ArrowRight;
        case SDLK_LEFT:         return Key::ArrowLeft;
        case SDLK_INSERT:       return Key::Insert;
        case SDLK_HOME:         return Key::Home;
        case SDLK_END:          return Key::End;
        case SDLK_PAGEUP:       return Key::PageUp;
        case SDLK_PAGEDOWN:     return Key::PageDown;
        case SDLK_F1:           return Key::F1;
        case SDLK_F2:           return Key::F2;
        case SDLK_F3:           return Key::F3;
        case SDLK_F4:           return Key::F4;
        case SDLK_F5:           return Key::F5;
        case SDLK_F6:           return Key::F6;
        case SDLK_F7:           return Key::F7;
        case SDLK_F8:           return Key::F8;
        case SDLK_F9:           return Key::F9;
        case SDLK_F10:          return Key::F10;
        case SDLK_F11:          return Key::F11;
        case SDLK_F12:          return Key::F12;
        case SDLK_F13:          return Key::F13;
        case SDLK_F14:          return Key::F14;
        case SDLK_F15:          return Key::F15;
        
        case SDLK_RSHIFT:       return Key::RightShift;
        case SDLK_LSHIFT:       return Key::LeftShift;
        case SDLK_RCTRL:        return Key::RightControl;
        case SDLK_LCTRL:        return Key::LeftControl;
        case SDLK_RALT:         return Key::RightAlt;
        case SDLK_LALT:         return Key::LeftAlt;
        
        case SDLK_SYSREQ:       return Key::SysRq;
        case SDLK_MENU:         return Key::AppMenu;
        case SDLK_POWER:        return Key::Power;
        default:                return Key::Unknown;
        }
        return Key::Unknown;
	}
    CEGUI::MouseButton GUI::SDLMouseToCEGUI(Uint8 mouseTipka) {
        using namespace CEGUI;
        switch (mouseTipka) {
        case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
        case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
        case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
        case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
        case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
        default: return CEGUI::MouseButton::NoButton;
        }
        return MouseButton::NoButton;
    }
	void GUI::OnSDLevent(SDL_Event& evnt) {
        CEGUI::utf32 codePoint;
		switch (evnt.type) {
		case SDL_MOUSEMOTION:
			_context->injectMousePosition((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
            _context->injectMouseButtonDown(SDLMouseToCEGUI(evnt.button.button));
			break;
		case SDL_MOUSEBUTTONUP:
            _context->injectMouseButtonUp(SDLMouseToCEGUI(evnt.button.button));
			break;
		case SDL_TEXTINPUT:
            codePoint = 0;{}
            for (int i = 0; evnt.text.text[i] != '\0'; i++) {
                codePoint = codePoint | ((CEGUI::utf32)*(unsigned char*)&evnt.text.text[i] << (i * 8));
            }
            _context->injectChar(codePoint);
			break;
		case SDL_KEYDOWN:
            _context->injectKeyDown(SDLKeyToCEGUI(evnt.key.keysym.sym));
			break;
		case SDL_KEYUP:
            _context->injectKeyUp(SDLKeyToCEGUI(evnt.key.keysym.sym));
			break;
		default:
			break;
		}
	}
    void GUI::update() {
        unsigned int protekloOdPrethodnogFramea; // u ms
        
        unsigned int nextTime = SDL_GetTicks();
        protekloOdPrethodnogFramea = nextTime - _lastTime;
        _lastTime = nextTime;
        
        _context->injectTimePulse((float)protekloOdPrethodnogFramea / 1000.0f);
    }
}