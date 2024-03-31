#pragma once
#include "gles.h"
#include <function/time.h>

namespace WuXing
{
class GLES {
    private:
    static inline GLuint gProgram;
    static inline GLuint gvPositionHandle;
    
    static inline const GLfloat gTriangleVertices[] = {0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f};
    
    static inline auto gVertexShader =
        "attribute vec4 vPosition;\n"
        "void main() {\n"
        "  gl_Position = vPosition;\n"
        "}\n";
    
    
    static inline auto gFragmentShader =
        "precision mediump float;\n"
        "void main() {\n"
        "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
        "}\n";
    

    static void printGLString(const char* name, GLenum s) {
        const char* v = (const char*)glGetString(s);
        LOGI("GL %s = %s\n", name, v);
    }
    
    static void checkGlError(const char* op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    static GLuint loadShader(GLenum shaderType, const char* pSource) {
        GLuint shader = glCreateShader(shaderType);
        if (shader) {
            glShaderSource(shader, 1, &pSource, NULL);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen) {
                    char* buf = (char*)malloc(infoLen);
                    if (buf) {
                        glGetShaderInfoLog(shader, infoLen, NULL, buf);
                        LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                        free(buf);
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
        return shader;
    }
    
    static GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
        if (!vertexShader) {
            return 0;
        }
    
        GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
        if (!pixelShader) {
              return 0;
        }
    
        GLuint program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength) {
                    char* buf = (char*)malloc(bufLength);
                    if (buf) {
                        glGetProgramInfoLog(program, bufLength, NULL, buf);
                        LOGE("Could not link program:\n%s\n", buf);
                        free(buf);
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }
    
    public:
    static bool setupGraphics(int w, int h) {
        printGLString("Version", GL_VERSION);
        printGLString("Vendor", GL_VENDOR); printGLString("Renderer", GL_RENDERER); printGLString("Extensions", GL_EXTENSIONS);
        LOGI("setupGraphics(%d, %d)", w, h);
        gProgram = createProgram(gVertexShader, gFragmentShader);
        if (!gProgram) {
            LOGE("Could not create program.");
            return false;
        }
        gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
        checkGlError("glGetAttribLocation");
        LOGI("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);
    
        glViewport(0, 0, w, h);
        checkGlError("glViewport");
        return true;
    }
    
    static void renderFrame() {
    
        static float grey;
        static bool dir = true;
        if(grey > 1.0f) dir = false;
        if(grey < 0.0f) dir = true;
        grey += dir?0.01f:-0.01f;
          
        glClearColor(grey, grey, grey, 1.0f);
        checkGlError("glClearColor");
    
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        checkGlError("glClear");
    
        glUseProgram(gProgram);
        checkGlError("glUseProgram");
    
        glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0,
                              gTriangleVertices);
        checkGlError("glVertexAttribPointer");
        glEnableVertexAttribArray(gvPositionHandle);
        checkGlError("glEnableVertexAttribArray");
        glDrawArrays(GL_TRIANGLES, 0, 3);
        checkGlError("glDrawArrays");
    }
};


} // namespace WuXing