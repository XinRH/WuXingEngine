#include "function/file.h"
#include "function/renderPass.h"

namespace WuXing
{
    class RENDER {
    public:
        struct RENDER_INFO {
            vector<float> bgColor = {0.f, 0.f, 0.f};
        };
    private:
        static inline vector<float> bgColor = {0.f,0.f,0.f};
    public:
        static void init(RENDER::RENDER_INFO info) {
            bgColor = info.bgColor;

            #ifdef WUXING_OpenGL
            RHI::renderRHI = new Rhi_OpenGL;
            #endif
            #ifdef WUXING_Vulkan
            RHI::renderRHI = new Rhi_Vulkan;
            #endif
            RHI::renderRHI->init();

            RENDERPASS::renderPass_PhyWorld = new RenderPass_PhyWorld;
            RENDERPASS::renderPass_Text = new RenderPass_Text;
            RENDERPASS::renderPass_PhyWorld->init();
            RENDERPASS::renderPass_Text->init();

        }
        static void tick() {
            RENDERPASS::renderPass_PhyWorld->prepareRenderData();
            RENDERPASS::renderPass_PhyWorld->swapBuffer();

            RHI::renderRHI->setBackgroundColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);

            RENDERPASS::renderPass_PhyWorld->draw();
            RENDERPASS::renderPass_Text->draw();
        }
        static void clear() {
            RENDERPASS::renderPass_Text->clear();
            RENDERPASS::renderPass_PhyWorld->clear();

            RHI::renderRHI->clear();
        }
    };
    
} // namespace WuXing
