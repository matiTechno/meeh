#pragma once

#include "GlObjects.hpp"
#include <glm/vec2.hpp>
#include <string>

namespace meeh
{

class Texture
{
public:
    Texture(const std::string& filename);
    Texture(const unsigned char* data, int size);
    Texture(GLenum internalFormat, int width, int height);

    void bind();
    glm::ivec2 getSize() const {return size;}

private:
    GlTexture glTexture;
    glm::ivec2 size;
};

} // meeh
