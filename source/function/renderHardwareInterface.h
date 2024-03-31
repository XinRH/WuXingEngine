#include <iostream>
#include "function/physics.h"

#ifdef WUXING_Vulkan
#include <Include/vulkan/vulkan.hpp>
#endif

namespace WuXing
{
    class Rhi;
    class RHI {
        public:
        static inline Rhi* renderRHI;
    };

    class Rhi {
        public:
        virtual void init() {};
        virtual void clear() {};

        virtual void setBackgroundColor(float, float, float, float) {};
        virtual void setPointSize(int size) {}
        virtual void drawElements(GEOMETRY::GeometryType type, int num) {};
        virtual void drawArrays(GEOMETRY::GeometryType type, int num) {};

        void initTextureOptions() {
            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Font Texture
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
        void setTextureOptions() {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    };

    #ifdef WUXING_OpenGL
    class Rhi_OpenGL : public Rhi {
    public:
        void init() {};
        void clear() {};

        void setBackgroundColor(float R, float G, float B, float A) {
            glClearColor(R, G, B, A);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        void setPointSize(int size) { glPointSize(10); }

        void drawElements(GEOMETRY::GeometryType type, int num) {
            switch (type)
            {
            case GEOMETRY::GeometryType::Points:
                glDrawElements(GL_POINTS, num, GL_UNSIGNED_INT, 0);
                break;
            case GEOMETRY::GeometryType::Rectangles:
                // numGeom个几何体,每个用两个三角形表示,供numGeom*2*3个顶点
                glDrawElements(GL_TRIANGLES, num*2*3, GL_UNSIGNED_INT, 0);
                break;
            default:
                break;
            }
        }
        void drawArrays(GEOMETRY::GeometryType type, int num) {
            switch (type)
            {
            case GEOMETRY::GeometryType::Triangles:
                glDrawArrays(GL_TRIANGLES, 0, 3 * num);
                break;
            default:
                break;
            }
        }
    };
    #endif
        
    #ifdef WUXING_Vulkan
    class Rhi_Vulkan : public Rhi {
    public:
        vk::Instance instance;
        vk::Device device;
        virtual void init() {
            vk::InstanceCreateInfo instanceCreateInfo;
            vk::ApplicationInfo applicationInfo;
            applicationInfo.setApiVersion(VK_API_VERSION_1_3);
            instanceCreateInfo.setPApplicationInfo(&applicationInfo);
            vk::Instance instance = vk::createInstance(instanceCreateInfo);

            vk::PhysicalDevice phyDevice;
            for(auto& device : instance.enumeratePhysicalDevices()) {
                phyDevice = device;
                std::cout<<phyDevice.getProperties().deviceName<<std::endl;
            }
        };
        virtual void clear() {
            device.destroy();
            instance.destroy();
        };
    };
    #endif


} // namespace WuXing
