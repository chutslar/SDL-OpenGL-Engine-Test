/* Important Resources:
 * - https://www.willusher.io/pages/sdl2/
 * - https://www.khronos.org/opengl/wiki/Tutorial1:_Creating_a_Cross_Platform_OpenGL_3.2_Context_in_SDL_(C_/_SDL)
 * - https://bcmpinc.wordpress.com/2015/08/18/creating-an-opengl-4-5-context-using-sdl2-and-glad/
 * - https://learnopengl.com/
 */
#include <cstdio>
#include <stdexcept>
#include <chrono>
#include <array>
#include <SDL.h>
#include <glad\glad.h>

#include "System.h"
#include "Utility.h"
#include "Shaders.h"
#include "Graphics.h"

typedef std::chrono::high_resolution_clock Clock;

// Global defines
constexpr int ERROR_MESSAGE_LENGTH = 200;

// SDL Initialization structs
struct SDL_Attributes {
	SDL_Window *window;
	SDL_GLContext glcontext;
};
struct Window_Attributes {
	const char* title;
	const int x, y, w, h;
	const Uint32 flags;
};

void initialize_SDL(SDL_Attributes &sdl, Window_Attributes &win_att);
void initialize_GL(SDL_Attributes &sdl);

int main(int, char**)
{
	// Initialization structures
	SDL_Attributes sdl;
	Window_Attributes win_att{
		"Hello World",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	};

	// Try to initialize SDL and OpenGL, catch failures by passing on message and quitting
	try
	{
		initialize_SDL(sdl, win_att);
		initialize_GL(sdl);
	}
	catch (std::exception& e)
	{
		printf("%s, \n", e.what());
		if (sdl.window) SDL_DestroyWindow(sdl.window);
		if (sdl.glcontext != NULL) SDL_GL_DeleteContext(sdl.glcontext);
		SDL_Quit();
		return 1;
	}

	// Compile shaders
	shader::Shaders shaders;

	camera::OrthographicCamera mainCam(800, 600, 100);

	// Initialize GraphicsManager
	graphics::GraphicsManager graphics(mainCam, shaders, win_att.w, win_att.h);


	/*              Main application code                  */

	// Make the backround a nice green-gray
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(sdl.window);

	std::string image_path = std::string(SDL_GetBasePath()) + std::string("res/awesomeface.png");
	image::Texture smile_texture(image_path.c_str());

	image_path = std::string(SDL_GetBasePath()) + std::string("res/SDL_logo.png");
	image::Texture sdl_texture(image_path.c_str());

	image_path = std::string(SDL_GetBasePath()) + std::string("res/cat.png");
	image::Texture cat_texture(image_path.c_str());

	ecs::System system(graphics);
	auto cat = system.addEntity(
		ecs::Drawable{ 
			ecs::Transform2DComponent{glm::vec3(0.5, 0, 0), 0}, 
			ecs::DrawComponent{glm::vec3(0.25,0.25, 1), &cat_texture}
		}
	);
	auto smile = system.addEntity(
		ecs::Drawable{
			ecs::Transform2DComponent{ glm::vec3(-0.5, 0, 0), 0 },
			ecs::DrawComponent{ glm::vec3(0.5,0.5, 1), &smile_texture }
		}
	);
	auto sdl_obj = system.addEntity(
		ecs::Drawable{
			ecs::Transform2DComponent{ glm::vec3(0, 0, 0), 0 },
			ecs::DrawComponent{ glm::vec3(1,1, 1), &sdl_texture }
		}
	);

	unsigned int selected = 0;

	auto get_selected = [cat, smile, sdl_obj](unsigned int selected)
	{
		switch (selected)
		{
			case 0:
				return smile;
			case 1:
				return sdl_obj;
			case 2:
				return cat;
			default:
				break;
		}
	};

	// Event container
	SDL_Event e;

	// Track whether application should quit
	bool shouldQuit = false;

	// Get time for counting frame deltaTime 
	auto time = Clock::now();

	// Game loop
	while (!shouldQuit) {
		
		// Process input events -- closing window, gamepad/keyboard/mouse, etc.
		while (SDL_PollEvent(&e)) {
			switch (e.type)
			{
				case SDL_QUIT:
					shouldQuit = true;
					break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							system.getComponent(get_selected(selected))->transform.position.y = 0.f;
							if (selected == 0) selected = 2;
							else selected = (selected - 1) % 3;
							break;
						case SDLK_RIGHT:
							system.getComponent(get_selected(selected))->transform.position.y = 0.f;
							selected = (selected + 1) % 3;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		auto deltaTime_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - time).count() / 1000;
		time = Clock::now();

		printf("FPS: %lld\n", 1000000 / deltaTime_ms);
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		switch (selected)
		{
			case 0:
				system.getComponent(smile)->transform.position.y = (float)(0.5*sin(0.001*SDL_GetTicks()));
				break;
			case 1:
				system.getComponent(sdl_obj)->transform.position.y = (float)(0.5*sin(0.001*SDL_GetTicks()));
				break;
			case 2:
				system.getComponent(cat)->transform.position.y = (float)(0.5*sin(0.001*SDL_GetTicks()));
				break;
			default:
				break;
		}
		//system.getComponent(cat)->transform.position.y = (float)(0.5*sin(-0.001*SDL_GetTicks()));

		system.draw(deltaTime_ms);

		// Swap the window we've been drawing on onto the screen - NOTE: if V-sync is on this blocks until the swap is finished
		SDL_GL_SwapWindow(sdl.window); 
	}

	// SDL cleanup
	SDL_DestroyWindow(sdl.window);
	SDL_GL_DeleteContext(sdl.glcontext);
	SDL_Quit();

	return 0;
}

void initialize_SDL(SDL_Attributes &sdl, Window_Attributes &win_att)
{
	// Setup buffer for error messages
	char error_buffer[ERROR_MESSAGE_LENGTH];

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		snprintf(error_buffer, sizeof(error_buffer), "SDL_Init Error: %s", SDL_GetError());
		throw std::runtime_error(error_buffer);
	}

	sdl.window = SDL_CreateWindow(win_att.title, win_att.x, win_att.y, win_att.w, win_att.h, win_att.flags);
	if (sdl.window == nullptr) {
		snprintf(error_buffer, sizeof(error_buffer), "SDL_CreateWindow Error: %s", SDL_GetError());
		throw std::runtime_error(error_buffer);
	}
}

void initialize_GL(SDL_Attributes &sdl)
{
	// Set context to version 330 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	/* Turn on double buffering so we draw to a separate "window" and then swap 
	 * to avoid flicker */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Setup buffer for error messages
	char error_buffer[ERROR_MESSAGE_LENGTH];

	sdl.glcontext = SDL_GL_CreateContext(sdl.window);
	if (sdl.glcontext == NULL)
	{
		snprintf(error_buffer, sizeof(error_buffer), "SDL_GL_CreateContex Error: %s", SDL_GetError());
		throw std::runtime_error(error_buffer);
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	int w, h;
	SDL_GetWindowSize(sdl.window, &w, &h);
	glViewport(0, 0, w, h);

	// Synch buffer swap with monitor vertical refresh - V-sync
	SDL_GL_SetSwapInterval(1);
}
