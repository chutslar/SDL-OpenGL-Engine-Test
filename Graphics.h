#pragma once
#include <array>
#include "Shaders.h"
#include "Image.h"
#include "Camera.h"
#include "Utility.h"

namespace graphics
{
	// A color has four floats r, g, b, a in range 0-1 inclusive
	struct Color {
		float r, g, b, a;
	};

	class GraphicsManager
	{
	private:
		// Require collection of compiled shaders
		shader::Shaders shaders;

		// Camera for setting view/projection
		camera::Camera cam;

		// Vertex Buffer Object for sending vertex data to GPU
		unsigned int VBO;

		// Element Buffer Object for sending index data to GPU with indexed drawing
		unsigned int EBO;
		
		// Vertex Array Object for defining vertex attributes (what to do with input)
		unsigned int VAO; 

		// Screen information
		int screenWidth, screenHeight;

	public:
		GraphicsManager(camera::Camera camera, shader::Shaders &ns, int screenWidth, int screenHeight);
		~GraphicsManager();

		// Update for screen changes
		void changeScreenSize(int w, int h) { screenWidth = w; screenHeight = h; }

		// Get screen size in pixels
		const glm::vec2 getScreenSize() { glm::vec2(screenWidth, screenHeight); }

		// Drawing utility functions
		void draw_triangle(std::array<glm::vec3, 3> &vertices, Color color);
		void draw_rect(glm::vec3 &bottom_left, glm::vec3 &upper_right, Color color, float rotation_deg = 0);
		void draw_image(glm::vec3 center_pos, const glm::vec3 &scale, image::Texture* texture, float rotation_deg = 0);
	};
}
