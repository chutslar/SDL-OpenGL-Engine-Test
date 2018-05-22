#include <stdexcept>
#include "Shaders.h"

unsigned int shader::compile_shader(const std::string &source, GLenum shader_type)
{
	unsigned int shader;
	shader = glCreateShader(shader_type);
	
	const GLchar* source_cstr = source.c_str();
	glShaderSource(shader, 1, &source_cstr, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}

	return shader;
}

shader::Shaders::Shaders()
{
	// Could make this more efficient, but would not be generalized for default shader references
	for (int i = 0; i < 5; ++i) shaders.push_back(0);
	shaders[STANDARD_VERTEX] = compile_shader(shader::standardVertexShaderSource, GL_VERTEX_SHADER);
	shaders[STANDARD_TRANSFORM_VERTEX] = compile_shader(shader::standardTransformVertexShaderSource, GL_VERTEX_SHADER);
	shaders[STANDARD_FRAGMENT] = compile_shader(shader::standardFragmentShaderSource, GL_FRAGMENT_SHADER);
	shaders[IMAGE_VERTEX] = compile_shader(shader::imageVertexShaderSource, GL_VERTEX_SHADER);
	shaders[IMAGE_FRAGMENT] = compile_shader(shader::imageFragmentShaderSource, GL_FRAGMENT_SHADER);
}

shader::Shaders::~Shaders()
{
	for (auto& shader : shaders)
		glDeleteShader(shader);
}

const ShaderRef shader::Shaders::add_shader(const std::string &source, GLenum shader_type)
{
	shaders.push_back(compile_shader(source, shader_type));
	return shaders.size() - 1;
}

GLint shader::Shaders::get_shader(const ShaderRef ref)
{
	if (ref > shaders.size())
	{
		char msg[512];
		snprintf(msg, sizeof(msg), "Shader reference out of bounds: %d", ref);
		throw std::invalid_argument(msg);
	}
	return shaders[ref];
}
