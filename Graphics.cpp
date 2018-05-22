#include <cstdio>
#include <cmath>
#include "Shaders.h"
#include "Graphics.h"

#define PI 3.14159265

constexpr int FRAGMENT_SOURCE_LENGTH = 128;

graphics::GraphicsManager::GraphicsManager(camera::Camera camera, shader::Shaders &ns, int w, int h) : cam(camera), shaders(ns), screenWidth(w), screenHeight(h)
{
	// Initialize Vertex Array Object and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Initialize Vertex Buffer Object and bind it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Initialize Element Buffer Object
	glGenBuffers(1, &EBO);
}

graphics::GraphicsManager::~GraphicsManager()
{
	// Delete VAO and VBO now that they aren't needed
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void graphics::GraphicsManager::draw_triangle(std::array<glm::vec3, 3> &vertices, Color color)
{
	// Vertex data
	float gl_vertices[] = {
		vertices[0].x, vertices[0].y, vertices[0].z,
		vertices[1].x, vertices[1].y, vertices[1].z,
		vertices[2].x, vertices[2].y, vertices[2].z
	};

	glm::mat4 trans(1);

	// Buffer vertex data with VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(gl_vertices), gl_vertices, GL_STATIC_DRAW);

	// Define vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Shader Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaders.get_shader(STANDARD_VERTEX)); // compiled_shaders.standardVertexShader
	glAttachShader(shaderProgram, shaders.get_shader(STANDARD_FRAGMENT)); // compiled_shaders.standardFragmentShader
	glLinkProgram(shaderProgram);

	/* Check program compiled correctly
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}
	*/

	// Use shader program
	glUseProgram(shaderProgram);

	// Set tgtColor param for fragment shader so we have the right color
	GLint colorLoc = glGetUniformLocation(shaderProgram, "tgtColor");
	glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

	// Set transform matrix for vertex shader to have correct rotation
	GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// Draw to window -- will still need to swap buffers to change what's on screen
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Delete the program now it is finished
	glDeleteProgram(shaderProgram);
}

void graphics::GraphicsManager::draw_rect(glm::vec3 &bottom_left, glm::vec3 &upper_right, Color color, float rotation_deg)
{
	/*
	glm::vec3 corner0;
	glm::vec3 corner1;
	glm::vec3 corner2;
	glm::vec3 corner3;
	if (rotation_deg)
	{
		float rad = (float)(rotation_deg * PI / 180.0);
		float cos_t = cos(rad);
		float sin_t = sin(rad);
		corner0 = glm::vec3(upper_right.x*cos_t - upper_right.y*sin_t, upper_right.x*sin_t + upper_right.y*cos_t, 0.f);
		corner1 = glm::vec3(upper_right.x*cos_t - bottom_left.y*sin_t, upper_right.x*sin_t + bottom_left.y*cos_t, 0.f);
		corner2 = glm::vec3(bottom_left.x*cos_t - bottom_left.y*sin_t, bottom_left.x*sin_t + bottom_left.y*cos_t, 0.f);
		corner3 = glm::vec3(bottom_left.x*cos_t - upper_right.y*sin_t, bottom_left.x*sin_t + upper_right.y, 0.f);
	}
	else
	{
		corner0 = upper_right;
		corner1 = glm::vec3(upper_right.x, bottom_left.y, 0.f);
		corner2 = bottom_left;
		corner3 = glm::vec3(bottom_left.x, upper_right.y, 0.f);
	}
	*/


	float vertices[] = {
		upper_right.x, upper_right.y, 0.0f,
		upper_right.x, bottom_left.y, 0.0f,  // bottom right
		bottom_left.x, bottom_left.y, 0.0f,
		bottom_left.x, upper_right.y, 0.0f   // top left 
	};
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Transformation matrix
	glm::mat4 trans;

	// Add rotation by rotation_deg in Z-axis to transformation matrix
	trans = glm::rotate(trans, glm::radians(rotation_deg), glm::vec3(0, 0, 1));

	// Buffer vertex data with VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Buffer index data with EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Shader Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaders.get_shader(STANDARD_TRANSFORM_VERTEX)); // compiled_shaders.standardVertexShader
	glAttachShader(shaderProgram, shaders.get_shader(STANDARD_FRAGMENT)); // compiled_shaders.standardFragmentShader
	glLinkProgram(shaderProgram);

	/* Check program compiled correctly
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}
	*/

	// Use shader program
	glUseProgram(shaderProgram);

	// Set tgtColor param for fragment shader so we have the right color
	GLint colorLoc = glGetUniformLocation(shaderProgram, "tgtColor");
	glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

	// Set transform matrix for vertex shader to have correct rotation
	GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// Draw to window -- will still need to swap buffers to change what's on screen
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Delete the program now it is finished
	glDeleteProgram(shaderProgram);
}

void graphics::GraphicsManager::draw_image(glm::vec3 center_pos, const glm::vec3 &scale, image::Texture* texture, float rotation_deg)
{
	if (!texture)
	{
		printf("Texture not found\n");
		return;
	}
	float glWidth = 2 * texture->width / (float)screenWidth;
	float glHeight = 2 * texture->height / (float)screenHeight;
	float midX = glWidth / 2.f;
	float midY = glHeight / 2.f;
	center_pos -= (glm::vec3(midX, midY, 0)) * scale;
	float vertices[] = {
		// positions						// colors           // texture coords
		glWidth, glHeight, center_pos.z,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		glWidth, 0.0f, center_pos.z,	    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		0.0f, 0.0f, center_pos.z,	        0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		0.0f, glHeight, center_pos.z,	    1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Transformation matrix
	glm::mat4 trans;

	// Add rotation by rotation_deg in Z-axis to transformation matrix
	trans = glm::rotate(trans, glm::radians(rotation_deg), glm::vec3(0, 0, 1));
	
	// Translate center to center_pos
	trans = glm::translate(trans, center_pos);
	
	// Scale by scale factor
	trans = glm::scale(trans, scale);

	// Buffer vertex data with VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Buffer index data with EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Bind VAO for defining attributes
	glBindVertexArray(VAO);

	// Define position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Define color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Define texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Set texture wrap and scaling parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Shader Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaders.get_shader(IMAGE_VERTEX)); // compiled_shaders.standardVertexShader
	glAttachShader(shaderProgram, shaders.get_shader(IMAGE_FRAGMENT)); // compiled_shaders.standardFragmentShader
	glLinkProgram(shaderProgram);

	/* Check program compiled correctly */
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}
	

	// Use shader program
	glUseProgram(shaderProgram);

	// Load the texture into OpenGL
	unsigned int tex = texture->load_gl();

	// Activate the texture unit before binding
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, tex);
	
	// Set transform matrix for vertex shader to have correct rotation
	GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw to window -- will still need to swap buffers to change what's on screen
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Delete the program now it is finished
	glDeleteProgram(shaderProgram);
}
