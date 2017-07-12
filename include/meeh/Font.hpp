#pragma once

#include "Texture.hpp"
#include <array>
#include <string>

namespace meeh
{

struct Font
{
    struct Glyph
    {};
    Texture texture;
    int newLinespace;
    std::array<Glyph, 126> glyphs;
};

Font loadFont(const std::string& filename);

} // meeh
