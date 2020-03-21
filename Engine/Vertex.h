#pragma once
#include<GL/glew.h>
#include<vector>
namespace Engine {
	struct UV {
		float u;
		float v;
	};
	struct Vertex {
		//float position[2];
		struct Position {
			float x;
			float y;
		} position;


		//rgba8 - 8 bits per channel
		struct ColorRGBA8 {
			ColorRGBA8() : r(0), g(0), b(0), a(0) {	}

			ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : 
				r(R), g(G), b(B), a(A) {	}

			// redom stavljaj boje na vrhove koliko ih dobijes kao parametar vektor colors .size()
			void SetColor(GLubyte R, GLubyte G, GLubyte B, GLubyte A) {
				r = R; g = G; b = B; a = A;
			}
			static std::vector<ColorRGBA8> getVectorWithSameColor(ColorRGBA8 color) {
				std::vector<ColorRGBA8> colors;
				for (int i = 0; i < 4; i++) { 
					colors.push_back(color);
				}
				return colors;
			}
			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;
		} color;

		//UV Texture coorinates
		UV uv; //[0,1]

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}
	};
}