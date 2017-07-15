#include <meeh/GlObjects.hpp>
#include <meeh/glad.h>
#include <assert.h>

namespace meeh
{

GlTexture::GlTexture() {glGenTextures(1, &id);}
GlTexture::~GlTexture() {if(wasMoved == false) glDeleteTextures(1, &id);}

GlTexture::GlTexture(GlTexture&& rhs): id(rhs.id) {rhs.wasMoved = true;}

GlTexture& GlTexture::operator=(GlTexture&& rhs)
{
    assert(&rhs != this);
    this->~GlTexture();
    wasMoved = rhs.wasMoved;
    id = rhs.id;
    rhs.wasMoved = true;
    return *this;
}

GlShader::GlShader(GLenum type) {id = glCreateShader(type);}
GlShader::~GlShader() {glDeleteShader(id);}

GlProgram::GlProgram() {id = glCreateProgram();}
GlProgram::~GlProgram() {glDeleteProgram(id);}

GlVAO::GlVAO() {glGenVertexArrays(1, &id);}
GlVAO::~GlVAO() {glDeleteVertexArrays(1, &id);}

GlBO::GlBO() {glGenBuffers(1, &id);}
GlBO::~GlBO() {glDeleteBuffers(1, &id);}

GlSampler::GlSampler() {glGenSamplers(1, &id);}
GlSampler::~GlSampler() {glDeleteSamplers(1, &id);}

GlFramebuffer::GlFramebuffer() {glGenFramebuffers(1, &id);}
GlFramebuffer::~GlFramebuffer() {glDeleteFramebuffers(1, &id);}

} // meeh
