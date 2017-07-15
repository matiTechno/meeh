#pragma once

#include "Texture.hpp"
#include <array>
#include <string>
#include <glm/vec4.hpp>

namespace meeh
{

struct Font
{
    struct Glyph
    {
        int advance;
        glm::ivec2 offset;
        glm::ivec4 texCoords;
    };
    Texture texture;
    int newLinespace;
    int ascent;
    int descent;
    std::array<Glyph, 126> glyphs;
};

Font loadFont(const std::string& filename);
Font loadFont(unsigned char* data);

} // meeh
