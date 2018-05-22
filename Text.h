#pragma once
#include <vector>
#include <array>
#include <glad/glad.h>
#include "Utility.h"

// Forward declaration of FreeType types
typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

using FontRef = unsigned int;

namespace text
{
	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};

	using CharacterArray = Character[128];

	class FontManager
	{
	private:
		FT_Library *ft;
		std::vector<FT_Face> fonts;
		std::vector<CharacterArray> characters;

	public:
		FontManager();
		~FontManager();

		// Add new font
		const FontRef addFont(const char* fontname, int size);
		void drawString(FontRef font, const char* string, glm::vec2 position);
	};
}

