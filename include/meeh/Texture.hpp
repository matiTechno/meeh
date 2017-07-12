#pragma once

#include "GlObjects.hpp"
#include <glm/vec2.hpp>

namespace meeh
{

class Texture
{
public:
    glm::ivec2 getSize() const {return size;}

private:
    GlTexture glTexture;
    glm::ivec2 size;
};

} // meeh
