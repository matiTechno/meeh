#pragma once

#include "NonCpNonMv.hpp"
using GLuint = unsigned int;
using GLenum = unsigned int;

namespace meeh
{

class GlBase: public NonCpNonMv
{
public:
    GLuint getId() const {return id;}

protected:
    GLuint id;
};

class GlTexture
{
public:
    GlTexture();
    ~GlTexture();
    GlTexture(const GlTexture&) = delete;
    GlTexture& operator=(const GlTexture&) = delete;
    GlTexture(GlTexture&& rhs);
    GlTexture& operator=(GlTexture&& rhs);

private:
    bool wasMoved = false;
    GLuint id;
};

class GlShader: public GlBase
{
public:
    GlShader(GLenum type);
    ~GlShader();
};

class GlProgram: public GlBase
{
public:
    GlProgram();
    ~GlProgram();
};

class GlVAO: public GlBase
{
public:
    GlVAO();
    ~GlVAO();
};

class GlBO: public GlBase
{
public:
    GlBO();
    ~GlBO();
};

class GlSampler: public GlBase
{
    GlSampler();
    ~GlSampler();
};

class GlFramebuffer: public GlBase
{
    GlFramebuffer();
    ~GlFramebuffer();
};

} // meeh
