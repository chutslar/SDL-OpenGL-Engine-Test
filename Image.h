#pragma once
#include <glad\glad.h>

namespace image
{
	class Texture
	{
	public:
		// Information about the image
		int width, height, nrChannels;

		// Image data
		unsigned char* data;

		// Texture after binding
		unsigned int tex;

		// Image format
		GLenum format;

		// How to handle wrapping image and scaling down
		GLint wrap, scale;
		
		Texture(const char* filename, GLint wrap_behavior = GL_REPEAT, GLint scale_behavior = GL_LINEAR);
		~Texture();

		void load_texture_data(const char* filename);
		unsigned int load_gl();
	};
}