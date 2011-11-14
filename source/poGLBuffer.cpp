#include "poGLBuffer.h"

#ifdef POTION_IOS
	#define glMapBuffer glMapBufferOES
	#define glUnmapBuffer glUnmapBufferOES
#endif

poGLBufferConfig::poGLBufferConfig()
:	target(GL_ARRAY_BUFFER)
,	usage(GL_STREAM_DRAW)
{}

poGLBufferConfig::poGLBufferConfig(GLenum target)
:	target(target)
,	usage(GL_STREAM_DRAW)
{}

poGLBufferConfig::poGLBufferConfig(GLenum target, GLenum usage)
:	target(GL_ARRAY_BUFFER)
,	usage(GL_STREAM_DRAW)
{}




poGLBuffer::poGLBuffer(GLenum target, size_t size)
{
	glGenBuffers(1, &uid);
	setConfig(poGLBufferConfig(target));
	setSize(size);
}

poGLBuffer::poGLBuffer(poGLBufferConfig config, size_t size)
{
	glGenBuffers(1, &uid);
	setConfig(config);
	setSize(size);
}

poGLBuffer::~poGLBuffer() {
	glDeleteBuffers(1, &uid);
	uid = 0;
}

void poGLBuffer::bind() {
	glBindBuffer(config.target, uid);
}

void poGLBuffer::unbind() {
	glBindBuffer(config.target, 0);
}

void poGLBuffer::bufferData(size_t size, const void *data, GLenum usage) {
	glBufferData(config.target, size, data, config.usage);
}

void poGLBuffer::bufferSubData(size_t offset, size_t size, const void *data) {
	glBufferSubData(config.target, offset, size, data);
}

void *poGLBuffer::map(GLenum access) {
	return glMapBuffer(config.target, access);
}

void poGLBuffer::unmap() {
	glUnmapBuffer(config.target);
}

bool poGLBuffer::isValid() const {
	return uid > 0;
}

uint poGLBuffer::getUid() const {
	return uid;
}

uint poGLBuffer::getSize() const {
	return size;
}

poGLBufferConfig poGLBuffer::getConfig() const {
	return config;
}

void poGLBuffer::setSize(size_t sz) {
	size = sz;
	
	bind();
	glBufferData(config.target, sz, NULL, config.usage);
	unbind();
}

void poGLBuffer::setConfig(poGLBufferConfig c) {
	config = c;
}

void poGLBuffer::setTarget(GLenum target) {
	config.target = target;
}

void poGLBuffer::setUsage(GLenum usage) {
	config.usage = usage;
}


