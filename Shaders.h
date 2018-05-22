#pragma once
#include <string>
#include <vector>
#include <glad\glad.h>

#define STANDARD_VERTEX 0
#define STANDARD_TRANSFORM_VERTEX 1
#define STANDARD_FRAGMENT 2
#define IMAGE_VERTEX 3
#define IMAGE_FRAGMENT 4


using ShaderRef = unsigned int;

namespace shader
{
	// List all Shaders used here

	const std::string standardVertexShaderSource =
#include "standard.vert"
	;
	const std::string standardFragmentShaderSource =
#include "standard.frag"
	;
	const std::string standardTransformVertexShaderSource =
#include "standard_transform.vert"
		;
	const std::string imageVertexShaderSource =
#include "image.vert"
	;
	const std::string imageFragmentShaderSource =
#include "image.frag"
	;

	class Shaders
	{
	private:
		std::vector<unsigned int> shaders;
	public:
		Shaders();
		~Shaders();
		const ShaderRef add_shader(const std::string &source, GLenum shader_type);
		GLint get_shader(const ShaderRef ref);
	};

	unsigned int compile_shader(const std::string &source, GLenum shader_type);
}
