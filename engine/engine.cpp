#include "engine.h"
using namespace WuXing;

int main() {
    
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_Define);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_Time);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_Input);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_File);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_PhysicsWorld);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_Window);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_Camera);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_Font);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_RenderPipeline);
    SYSTEM::regFun(SYSTEM::funcType::initType, ENGINE::Init_Render);

    SYSTEM::regFun(SYSTEM::funcType::tickType, ENGINE::Tick_Time);
    SYSTEM::regFun(SYSTEM::funcType::tickType, ENGINE::Tick_PhysicsWorld);
    SYSTEM::regFun(SYSTEM::funcType::tickType, ENGINE::Tick_Window);
    SYSTEM::regFun(SYSTEM::funcType::tickType, ENGINE::Tick_Render);
    SYSTEM::regFun(SYSTEM::funcType::tickType, ENGINE::Tick_Camera);
    
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_Render);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_RenderPipeline);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_Camera);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_Window);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_PhysicsWorld);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_File);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_Input);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_Time);
    SYSTEM::regFun(SYSTEM::funcType::clearType, ENGINE::Clear_Define);

    SYSTEM::runFun(SYSTEM::funcType::initType);

    while(!ENGINE::shutDownEngineFlag) {
        SYSTEM::runFun(SYSTEM::funcType::tickType);
        if(TIME::sysTime() > 1.0f) break;
    }
    SYSTEM::runFun(SYSTEM::funcType::clearType);
    
    return 0;
}