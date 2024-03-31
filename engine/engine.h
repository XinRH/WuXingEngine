
#include "core/define.h"
#include "core/reflect.h"

#ifdef WUXING_EnTT
#include "core/esc.h"
#endif

#include "function/camera.h"
#include "function/render.h"
#include "function/time.h"
#include "function/window.h"
#include "function/file.h"

using namespace std;

namespace WuXing
{
    class SYSTEM{
        #ifdef WUXING_EnTT
        private:
        struct initFunc { void(*func)(void); };
        struct tickFunc { void(*func)(void); };
        struct clearFunc { void(*func)(void); };
        static inline entt::registry funcReg;
        #endif
        
        public:
        enum funcType {
            initType = 1,
            tickType,
            clearType
        };
        static inline multimap<funcType, void(*)(void)> funMap;
        static void regFun(funcType type, void(*fun)(void)) {
            #ifndef WUXING_EnTT
            funMap.insert(pair<funcType, void(*)(void)>(type, fun));
            #endif

            #ifdef WUXING_EnTT
            auto funcEntity = funcReg.create();

            switch (type)
            {
            case funcType::initType:
                funcReg.emplace<initFunc>(funcEntity, fun);
                break;
            case funcType::tickType:
                funcReg.emplace<tickFunc>(funcEntity, fun);
                break;
            case funcType::clearType:
                funcReg.emplace<clearFunc>(funcEntity, fun);
                break;
            default:
                break;
            }
            #endif
        }
        static void runFun(funcType type) {
            #ifndef WUXING_EnTT
            multimap<funcType, void(*)(void)>::iterator itor;
            itor = funMap.find(type);
            for(int i = 0; i<funMap.count(type); i++, itor++) {
                itor->second();
            }
            #endif

            #ifdef WUXING_EnTT
            if(type == funcType::initType) {
                auto funcView = funcReg.view<initFunc>();
                for(auto entity: funcView) {
                    auto &f = funcView.get<initFunc>(entity);
                    f.func();
                }
            }
            if(type == funcType::tickType) {
                auto funcView = funcReg.view<tickFunc>();
                for(auto entity: funcView) {
                    auto &f = funcView.get<tickFunc>(entity);
                    f.func();
                }
            }
            if(type == funcType::clearType) {
                auto funcView = funcReg.view<clearFunc>();
                for(auto entity: funcView) {
                    auto &f = funcView.get<clearFunc>(entity);
                    f.func();
                }
            }
            #endif
        }
    };
    class ENGINE {
        public:
        static inline bool shutDownEngineFlag = false;
        static void Init_Define() {
            cout<<__FUNCTION__<<endl;
            DEFINE::init();
        }
        static void Init_Time() {
            cout<<__FUNCTION__<<endl;
            TIME::init();
        }
        static void Init_Input() {
            cout<<__FUNCTION__<<endl;
            INPUT::init();
        }
        static void Init_File() {
            FILE::FileInfo info;
            info.FilePath = string(_RESOURCE_PATH) + "/configs/usrDefault.xml";
            if(info.FilePath == FileXML::getFilePath()) { return; }
            cout<<__FUNCTION__<<endl;
            FileXML::init(info);
            FileFont::init();
        }
        static void Init_Font() {
            cout<<__FUNCTION__<<endl;
            FileFont::init();
        }
        static void Init_Window() {
            if(WINDOW::window != nullptr) return;
            Init_File();
            cout<<__FUNCTION__<<endl;


            #ifdef WUXING_SDL2
            WINDOW::window = new Window_SDL;
            #endif

            #ifdef WUXING_glfw
            WINDOW::window = new Window_GLFW;
            #endif

            WINDOW::WINDOW_INFO windowInfo;
            windowInfo.width = atoi(FileXML::findXML_Attribute({"Window", "Width"}).c_str());
            windowInfo.height = atoi(FileXML::findXML_Attribute({"Window", "Height"}).c_str());
            windowInfo.swapIntervalFlag = atoi(FileXML::findXML_Attribute({"Window", "SwapIntervalFlag"}).c_str());
            WINDOW::window->init(windowInfo);
        }
        static void Init_Camera() {
            cout<<__FUNCTION__<<endl;
            Init_File();

            vector<float> cameraPos, aimPos, speedVec;
            cameraPos.push_back(atof(FileXML::findXML_Attribute({"Camera", "cameraPosX"}).c_str()));
            cameraPos.push_back(atof(FileXML::findXML_Attribute({"Camera", "cameraPosY"}).c_str()));
            cameraPos.push_back(atof(FileXML::findXML_Attribute({"Camera", "cameraPosZ"}).c_str()));
            aimPos.push_back(atof(FileXML::findXML_Attribute({"Camera", "aimPosX"}).c_str()));
            aimPos.push_back(atof(FileXML::findXML_Attribute({"Camera", "aimPosY"}).c_str()));
            aimPos.push_back(atof(FileXML::findXML_Attribute({"Camera", "aimPosZ"}).c_str()));
            speedVec.push_back(atof(FileXML::findXML_Attribute({"Camera", "speedVecX"}).c_str()));
            speedVec.push_back(atof(FileXML::findXML_Attribute({"Camera", "speedVecY"}).c_str()));
            speedVec.push_back(atof(FileXML::findXML_Attribute({"Camera", "speedVecZ"}).c_str()));
            
            CAMERA3D::CameraInfo info;
            info.cameraPos = cameraPos;
            info.aimPos = aimPos;
            info.speedVec = speedVec;
            CAMERA3D::init(info);
        }
        static void Init_PhysicsWorld() {
            if(PARTICLES::getNum()) return;

            Init_File();
            cout<<__FUNCTION__<<endl;
            PHYSICS::dt = atof(FileXML::findXML_Attribute({"Physics", "DeltaTime"}).c_str());

            std::vector<float> particles;
            int row = 10, line = row, high = row;
            
            int dim = 3;
            for(int i = 0; i <= row; i++) {
                for(int j = 0; j <= line; j++) {
                    for(int k = 0; k <= high; k++) {
                        float pos[] = {i * 1.f/row - 0.5f, j * 1.f/line - 0.5f, k * 1.f/high - 0.5f};
                        particles.insert(particles.end(), pos, pos + dim);
                    }
                }
            }
            PARTICLES::init(dim);
            PARTICLES::setPosData(particles);
            PARTICLES::setVecData(particles);
        }
        static void Init_RenderPipeline() {
            Init_File();
            
            FILE::FileInfo info;
            info.FilePath = string(_RESOURCE_PATH) + "/shaders/phyWorld.vert";
            FileTXT::init(info);

            vector<RenderPipeline::shaderSourceInfo> infos;
            RenderPipeline::shaderSourceInfo vertInfo;
            vertInfo.type = RenderPipeline::shaderType::vertexShader;
            vertInfo.source = FileTXT::getContent();
            infos.push_back(vertInfo);

            info.FilePath = string(_RESOURCE_PATH) + "/shaders/phyWorld.frag";
            FileTXT::init(info);
        
            RenderPipeline::shaderSourceInfo fragInfo;
            fragInfo.type = RenderPipeline::shaderType::fragmentShader;
            fragInfo.source = FileTXT::getContent();
            infos.push_back(fragInfo);

            if(RenderPipeline::getShaderProgram("phyWorld") || RenderPipeline::getShaderProgram("text")) return;
            cout<<__FUNCTION__<<endl;
            RenderPipeline::init();
            RenderPipeline::add("phyWorld", infos);

            info.FilePath = string(_RESOURCE_PATH) + "/shaders/text.vert";
            FileTXT::init(info);
            infos[0].source = FileTXT::getContent();
            info.FilePath = string(_RESOURCE_PATH) + "/shaders/text.frag";
            FileTXT::init(info);
            infos[1].source = FileTXT::getContent();
            RenderPipeline::add("text", infos);
        }
        static void Init_Render() {
            Init_File();
            Init_PhysicsWorld();
            Init_Window();
            Init_RenderPipeline();
            cout<<__FUNCTION__<<endl;

            RENDER::RENDER_INFO renderInfo;
            renderInfo.bgColor[0] = atof(FileXML::findXML_Attribute({"Render", "bgColor0"}).c_str());
            renderInfo.bgColor[1] = atof(FileXML::findXML_Attribute({"Render", "bgColor1"}).c_str());
            renderInfo.bgColor[2] = atof(FileXML::findXML_Attribute({"Render", "bgColor2"}).c_str());
            RENDER::init(renderInfo);
        }
        static void Tick_Time() {
            TIME::tick();
        }
        static void Tick_Window() {
            if(WINDOW::window->isClosed()) shutDownEngineFlag = true;
            WINDOW::window->setTitle(string("FPS : ") + to_string(1.f / TIME::deltaTime()));
            WINDOW::window->tick();
        }
        static void Tick_PhysicsWorld() {
            PARTICLES::tick();
        }
        static void Tick_Render() {
            RENDER::tick();
        }
        static void Tick_Camera() {
            CAMERA3D::tick();
        }
        static void Clear_Render() {
            cout<<__FUNCTION__<<endl;
            RENDER::clear();
            Clear_RenderPipeline();
        }
        static void Clear_RenderPipeline() {
            if(!RenderPipeline::getShaderProgram("phyWorld") || !RenderPipeline::getShaderProgram("text")) return;
            cout<<__FUNCTION__<<endl;
            RenderPipeline::clear();
        }
        static void Clear_Camera() {
            cout<<__FUNCTION__<<endl;
            CAMERA3D::clear();
        }
        static void Clear_Window() {
            cout<<__FUNCTION__<<endl;
            WINDOW::window->clear();
        }
        static void Clear_PhysicsWorld() {
            cout<<__FUNCTION__<<endl;
            PARTICLES::clear();
        }
        static void Clear_File() {
            FileTXT::clear();
            FileFont::clear();
            FileXML::clear();
            cout<<__FUNCTION__<<endl;
        }
        static void Clear_Input() {
            cout<<__FUNCTION__<<endl;
            INPUT::clear();
        }
        static void Clear_Time() {
            cout<<__FUNCTION__<<endl;
        }
        static void Clear_Define() {
            cout<<__FUNCTION__<<endl;
        }
    };
} // namespace WuXing

