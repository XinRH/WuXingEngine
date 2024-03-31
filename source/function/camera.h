#include "function/window.h"
#include "function/renderPipeline.h"
#include "function/time.h"
#include "function/input.h"

#ifdef WUXING_glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace WuXing
{
    class CAMERA3D {
    private:
        static inline glm::vec3 cameraPos, aimPos, speedVec;
        static inline glm::vec3 cameraForwardDir, cameraUpDir, cameraRightDir;
        static inline glm::vec3 worldUpDir = glm::vec3(0.f, 1.f, 0.f);
        static inline glm::mat4 viewMat, projectionMat, transformMat;
    public:
        struct CameraInfo {
            vector<float> cameraPos;
            vector<float> aimPos;
            vector<float> speedVec;
        };
        static void init(CameraInfo info) {
            cameraPos = glm::vec3(info.cameraPos[0], info.cameraPos[1], info.cameraPos[2]);
            aimPos = glm::vec3(info.aimPos[0], info.aimPos[1], info.aimPos[2]);
            speedVec = glm::vec3(info.speedVec[0], info.speedVec[1], info.speedVec[2]);
            cameraForwardDir = glm::normalize(aimPos - cameraPos);
            glm::vec3 worldUpDir = glm::vec3(0.f, 1.f, 0.f);
        }
        static void tick() {
            if(INPUT::isPressed(INPUT::Command::up))        cameraPos += speedVec[0] * cameraUpDir;
            if(INPUT::isPressed(INPUT::Command::down))      cameraPos -= speedVec[0] * cameraUpDir;
            if(INPUT::isPressed(INPUT::Command::left))      cameraPos -= speedVec[1] * cameraRightDir;
            if(INPUT::isPressed(INPUT::Command::right))     cameraPos += speedVec[1] * cameraRightDir;
            if(INPUT::isPressed(INPUT::Command::forward))   cameraPos += speedVec[1] * cameraForwardDir;
            if(INPUT::isPressed(INPUT::Command::backward))  cameraPos -= speedVec[1] * cameraForwardDir;
            
            cameraRightDir = glm::normalize(glm::cross(worldUpDir, -cameraForwardDir));
            cameraUpDir = glm::normalize(glm::cross(-cameraForwardDir, cameraRightDir));
            
            int width, height;
            WINDOW::window->getSize(width, height);
            transformMat = glm::mat4(1.0f);
            
            viewMat = glm::lookAt(cameraPos, cameraPos + cameraForwardDir, cameraUpDir);
            projectionMat = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
            transformMat = glm::rotate(transformMat, (float)TIME::sysTime() * 0.5f, glm::vec3(0.0f, 1.0f, 1.0f));
            
        }
        static glm::mat4 getViewMat() { return viewMat; }
        static glm::mat4 getProjectionMat() { return projectionMat; }
        static glm::mat4 getTransformMat() { return transformMat; }
        static void clear() {}
    };
} // namespace WuXing
