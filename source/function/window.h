#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "glad.h"

#ifdef WUXING_SDL2
#include "SDL/SDL.h"
#include "SDL/SDL_vulkan.h"
#undef main
#endif


#ifdef WUXING_glfw
#include "glfw3.h"
#endif

#include "function/input.h"

namespace WuXing
{
    class Window;
    #ifdef WUXING_SDL2
    class Window_SDL;
    #endif
    #ifdef WUXING_glfw
    class Window_GLFW;
    #endif
    class WINDOW {
        public:
            static inline Window* window = nullptr;
            struct WINDOW_INFO {
                int width = 400;
                int height = 400;
                int swapIntervalFlag = 1;
                std::string renderAPI = "OpenGL";
            };
    };
    class Window {
        public:
        virtual void init(WINDOW::WINDOW_INFO info) {}
        virtual void tick() {}
        virtual void clear() {}
        virtual bool isClosed() { return true; }
        virtual void setTitle(const std::string title) {}
        virtual void getSize(int& width, int& height) {}
    };
    
    #ifdef WUXING_glfw
    class Window_GLFW : public Window {
        private:
            GLFWwindow* m_GLFWwindow;
        public:
            virtual void init(WINDOW::WINDOW_INFO info) {
                glfwInit();
                m_GLFWwindow = glfwCreateWindow(info.width,info.height,"GLFWwindow",nullptr,nullptr);
                glfwMakeContextCurrent(m_GLFWwindow);
                glfwSetKeyCallback(m_GLFWwindow, INPUT::key_callback);
                gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
                glfwSwapInterval(info.swapIntervalFlag);
            }
            virtual void tick() {
                glfwSwapBuffers(m_GLFWwindow);
                glfwPollEvents();
            }
            virtual void clear()                            { glfwTerminate(); }
            virtual bool isClosed()                         { return glfwWindowShouldClose(m_GLFWwindow); }
            virtual void setTitle(const std::string title)  { glfwSetWindowTitle(m_GLFWwindow, (std::string("GLFW_Window : ") + title).c_str()); }
            GLFWwindow* getWindow()                         { return m_GLFWwindow; }
            void getSize(int& width, int& height)           { glfwGetWindowSize(m_GLFWwindow, &width, &height); }
    };
    #endif

    #ifdef WUXING_SDL2
    class Window_SDL : public Window {
        private:
            SDL_Window* m_SDLwindow;
            SDL_GLContext m_SDLcontext;
            SDL_Event m_windowEvent;
        public:
            virtual void init(WINDOW::WINDOW_INFO info) {
                SDL_Init(SDL_INIT_VIDEO);

                SDL_WindowFlags apiFlag;
                if(info.renderAPI == "Vulkan") { apiFlag = SDL_WINDOW_VULKAN; }
                else { apiFlag = SDL_WINDOW_OPENGL; }

                m_SDLwindow = SDL_CreateWindow("SDL2window",200,200,info.width,info.height,apiFlag);
                m_SDLcontext = SDL_GL_CreateContext(m_SDLwindow);
                SDL_GL_MakeCurrent(m_SDLwindow, m_SDLcontext);
                gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
                SDL_GL_SetSwapInterval(info.swapIntervalFlag);
            }
            virtual void tick() {
                SDL_GL_SwapWindow(m_SDLwindow);
                SDL_PollEvent(&m_windowEvent);
            }
            virtual void clear() {
                SDL_GL_DeleteContext(m_SDLcontext);
                SDL_DestroyWindow(m_SDLwindow);
                SDL_Quit();
            }
            virtual bool isClosed() {
                if (m_windowEvent.type == SDL_QUIT) {
                    m_windowEvent.type = SDL_FIRSTEVENT;
                    return true;
                }
                return false;
            }
            virtual void setTitle(const std::string title)  { SDL_SetWindowTitle(m_SDLwindow,(std::string("SDL_Window : ") + title).c_str()); }
            SDL_Window* getWindow() { return m_SDLwindow; }
            void getSize(int& width, int& height) {
                SDL_DisplayMode dm;

                if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
                {
                    SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
                    return;
                }
                width = dm.w;
                height = dm.h;
            }
    };
    #endif
} // namespace WuXing