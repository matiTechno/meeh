#include <meeh/Font.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include "imgui/stb_truetype.h"

namespace meeh
{

Font loadFont(const std::string& filename)
{
    (void)filename;
    return {};
}

Font loadFont(unsigned char* data)
{
    (void)data;
    return {};
}

} // meeh
