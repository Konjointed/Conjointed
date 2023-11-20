#ifndef BUFFERS_H
#define BUFFERS_H

#include "IncludeGL.h"

class FrameBuffer {
public:
    static unsigned int CreateFrameBuffer() {
        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        return framebuffer;
    }

    static unsigned int CreateTextureAttachment(int width, int height) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }

    static unsigned int CreateRenderBufferAttachment(int width, int height) {
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        return rbo;
    }

    //unsigned int fbo;
    //unsigned int rbo;
    //unsigned int texture;
private:
};

#endif