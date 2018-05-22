#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include "Image.h"

image::Texture::Texture(const char * filename, GLint wrap_behavior, GLint scale_behavior) :
	wrap(wrap_behavior), scale(scale_behavior)
{
	load_texture_data(filename);
}

image::Texture::~Texture()
{
	// Free the image memory
	stbi_image_free(data);

	// Delete the texture
	glDeleteTextures(1, &tex);
}

void image::Texture::load_texture_data(const char * filename)
{
	stbi_set_flip_vertically_on_load(true);

	// Load data from image into an array
	data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		printf("Could not load texture from file: %s\n", filename);
		return;
	}
	switch (nrChannels)
	{
		case 1: format = GL_ALPHA; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
	}
	/* Start of cut code
	if (!data)
	{
		printf("Could not load texture into OpenGL - no image data loaded yet\n");
		return 0;
	}
	*/
	// Generate a new texture for the image
	//unsigned int tex;
	glGenTextures(1, &tex);

	// Activate the texture unit before binding
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture for further commands
	glBindTexture(GL_TEXTURE_2D, tex);

	// Loading the image data
	glTexImage2D(
		GL_TEXTURE_2D, // Work on currently bound 2D texture 
		0, // Use base level for mipmaps
		format, // Image color format 
		width, height, // Width and height extracted from image
		0, // Border - legacy
		format, // Format of the data
		GL_UNSIGNED_BYTE, // Type of the data - byte = char
		data // Image data we loaded earlier
	);

	// Generate mipmaps for the texture
	glGenerateMipmap(GL_TEXTURE_2D);
}

unsigned int image::Texture::load_gl()
{
	// Activate the texture unit before binding
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture for further commands
	glBindTexture(GL_TEXTURE_2D, tex);
	
	// Update the image texture - no real runtime cost if nothing has changed
	glTexSubImage2D(
		tex, // Work on currently bound 2D texture 
		0, // Use base level for mipmaps
		0, // x Texel offset 
		0, // y Texel offset
		width, height, // Width and height extracted from image
		format, // Format of the data
		GL_UNSIGNED_BYTE, // Type of the data - byte = char
		data // Image data we loaded earlier
	);

	return tex;
}
