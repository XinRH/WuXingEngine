#pragma once

#include <iostream>
using namespace std;

// DEFINE HEADERS
#include "resource/generated/define/define_tinyxml2.h"
#include "resource/generated/define/define_freetype.h"
#include "resource/generated/define/define_glfw.h"
#include "resource/generated/define/define_SDL2.h"
#include "resource/generated/define/define_glad.h"
#include "resource/generated/define/define_OpenGL.h"
#include "resource/generated/define/define_EnTT.h"
#include "resource/generated/define/define_glm.h"
// define headers
namespace WuXing
{
class DEFINE {
    public:
    static void init() {
        cout<<"------3rdPartyLibs used as followed:------"<<endl;
        // DEFINES
		cout<<"tinyxml2"<<endl;
		cout<<"freetype"<<endl;
		cout<<"glfw"<<endl;
		cout<<"SDL2"<<endl;
		cout<<"glad"<<endl;
		cout<<"OpenGL"<<endl;
		cout<<"EnTT"<<endl;
		cout<<"glm"<<endl;
        // defines
        cout<<"------3rdPartyLibs used as shown:------"<<endl;
		}
	};
}
