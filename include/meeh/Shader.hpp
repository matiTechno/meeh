#pragma once

#include "GlObjects.hpp"
#include <string>
#include <map>
using GLint = int;

namespace meeh
{

// shader source format:
// #VERTEX
// #FRAGMENT
// #GEOMETRY
// #COMPUTE

class Shader
{
public:
    Shader(const std::string& source, const std::string& idName);
    Shader(const std::string& filename);

    void bind();
    GLint getUniLocation(const std::string& uniName);

private:
    GlProgram glProgram;
    std::map<std::string, GLint> uniLocations;
    std::string idName;
};

} // meeh
