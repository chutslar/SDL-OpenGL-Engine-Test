#include <cstdio>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Text.h"

text::FontManager::FontManager()
{
	if (FT_Init_FreeType(ft))
		printf("ERROR::FREETYPE: Could not init FreeType Library");

}

text::FontManager::~FontManager()
{
	FT_Done_FreeType(*ft);
}

const FontRef text::FontManager::addFont(const char * fontname, int size)
{
	FT_Face face;
	if (FT_New_Face(*ft, fontname, 0, &face))
		printf("ERROR::FREETYPE: Failed to load font: %s\n", fontname);
	fonts.push_back(face);
	FT_Set_Pixel_Sizes(face, 0, size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	CharacterArray chars;
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("ERROR::FREETYTPE: Failed to load Glyph for: %c in font %s", c, fontname);
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		chars[c] = character;
	}
	characters.push_back(chars);
	FT_Done_Face(face);
	return fonts.size()-1;
}

void text::FontManager::drawString(FontRef font, const char * string, glm::vec2 position)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
