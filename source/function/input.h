#pragma once
#include "core/define.h"
#include <map>
#ifdef WUXING_glfw
#include "glfw3.h"
#endif

namespace WuXing
{
class INPUT {
    public:
        enum Command : unsigned int {
            up          = 1 << 0,   //q
            down        = 1 << 1,   //e
            left        = 1 << 2,   //a
            right       = 1 << 3,   //d
            forward     = 1 << 4,   //w
            backward    = 1 << 5,   //s
        };
    private:
        static inline std::map<unsigned int, unsigned int> KeyMap;
        static inline unsigned int m_command = 0x00000000;
        static inline unsigned int completeCommand = 0xFFFFFFFF;

    public:
        static void init() {
            #ifdef WUXING_glfw
            KeyMap[GLFW_KEY_W] = Command::forward;
            KeyMap[GLFW_KEY_S] = Command::backward;
            KeyMap[GLFW_KEY_A] = Command::left;
            KeyMap[GLFW_KEY_D] = Command::right;
            KeyMap[GLFW_KEY_Q] = Command::up;
            KeyMap[GLFW_KEY_E] = Command::down;
            #endif
        }
        #ifdef WUXING_glfw
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
            switch (action) {
                case GLFW_PRESS:
                switch (key) {
                    case GLFW_KEY_ESCAPE:
                        glfwSetWindowShouldClose(window, GL_TRUE);
                        break;
                    case GLFW_KEY_W: case GLFW_KEY_S: case GLFW_KEY_A: case GLFW_KEY_D: case GLFW_KEY_Q: case GLFW_KEY_E:
                        m_command |= (unsigned int)KeyMap.find(key)->second;
                        break;
                    default:
                        cout<<"undef key pressed"<<endl;
                        break;
                }
                break;

                case GLFW_RELEASE:
                switch (key) {
                    case GLFW_KEY_ESCAPE:
                        break;
                    case GLFW_KEY_W: case GLFW_KEY_S: case GLFW_KEY_A: case GLFW_KEY_D: case GLFW_KEY_Q: case GLFW_KEY_E:
                        m_command &= (completeCommand ^ (unsigned int)KeyMap.find(key)->second);
                        break;
                    default:
                        cout<<"undef key released"<<endl;
                        break;
                }
                break;
                
                default:
                break;
            }
        }
        #endif
        static bool isPressed(Command cmd) { return cmd & m_command; }
        static void clear() { KeyMap.clear(); }
    };
} // namespace WuXing
