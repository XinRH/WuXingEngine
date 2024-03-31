#include "function/file.h"
#include <algorithm>

#ifndef _PRECOMPILE_PATH
#define _PRECOMPILE_PATH
#endif

#ifndef _ENGINE_PATH
#define _ENGINE_PATH
#endif

#ifndef _SOURCE_PATH
#define _SOURCE_PATH
#endif

#ifndef _COMPILELIBS_PATH
#define _COMPILELIBS_PATH
#endif

using namespace WuXing;
using namespace std;

enum typeToChangeScript {
	setCompiler,
	setAndroidMirror,

	setAndroidCmakeScript3rdPartyLib,
	setWindowsCmakeScript3rdPartyLib,
	linkAndroidCmakeScript3rdPartyLib,
	linkWindowsCmakeScript3rdPartyLib,
	linkWindowsCmakeListsScript3rdPartyLib,

	print3rdPartyLibMsg,

	setDefineFile,
	setDefineFileHeaders
};

struct PreCompileInfo {
	vector<string> elements;
	string cfgFilePath;
	string aimFilePath;
	string fromWhichLine; 
	string endWith;
	typeToChangeScript type;
};


void setCmakeScriptUsingUserCFG(PreCompileInfo info) {
    WuXing::FILE::FileInfo engine_info;
    engine_info.FilePath = info.aimFilePath;
    FileTXT::init(engine_info);

	int beginLine = 0, endLine = FileTXT::getNumOfLine();
	for(int i = 1; i <= FileTXT::getNumOfLine(); i++) {
		if(FileTXT::getStrFromLine(i) == info.fromWhichLine) beginLine = i;
		if(FileTXT::getStrFromLine(i) == info.endWith) endLine = i;
		if(beginLine && endLine != FileTXT::getNumOfLine()) break;
	}
	if(beginLine > endLine) return;
	
	string new_str;
	for(int i = 1; i < beginLine; i++) { new_str += FileTXT::getStrFromLine(i) + "\n"; }
	new_str += info.fromWhichLine + "\n";

	for(int i = 0; i<info.elements.size(); i++) {
		if(!info.elements[i].size()) continue;
		switch (info.type)
		{
		case typeToChangeScript::linkWindowsCmakeListsScript3rdPartyLib:
		
			if(info.elements[i] == "EnTT" || info.elements[i] == "OpenGL" || info.elements[i] == "Vulkan") {
				new_str += "include(${RESOURCE_PATH}/generated/cmake/" + info.elements[i] + ".cmake)" + "\n\n";
			}
			else {
				WuXing::FILE::FileInfo fileInfo;
				fileInfo.FilePath = string(_RESOURCE_PATH) + "/configs/usrCompiler.xml";
				FileXML::init(fileInfo);
				if(FileXML::findXML_Attribute({"WINDOWS", "Link", "type"}) == "dynamic") {
					if(info.elements[i] == "SDL2") {
						new_str += "add_library(" + info.elements[i] + " MODULE IMPORTED)" + "\n";
						new_str += "set_target_properties(" + info.elements[i] + " PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/bin/" + info.elements[i] + ".dll)" + "\n";
						new_str += "target_link_libraries(${PROJECT_NAME} " + info.elements[i] + ")" + "\n\n";
						continue;
					}
				}
				new_str += "add_library(" + info.elements[i] + " STATIC IMPORTED)" + "\n";
				if(info.elements[i] == "SDL2") { new_str += "set_target_properties(" + info.elements[i] + " PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/lib" + info.elements[i] + ".dll.a)" + "\n"; }
				else if(info.elements[i] == "glfw") { new_str += "set_target_properties(" + info.elements[i] + " PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/lib" + info.elements[i] + "3.a)" + "\n"; }
				else { new_str += "set_target_properties(" + info.elements[i] + " PROPERTIES IMPORTED_LOCATION ${WUXINGENGINE_PATH}/build/win/lib/lib" + info.elements[i] + ".a)" + "\n"; }
				new_str += "target_link_libraries(${PROJECT_NAME} " + info.elements[i] + ")" + "\n\n";
			}
			break;
		case typeToChangeScript::setWindowsCmakeScript3rdPartyLib:
			new_str += "add_definitions(-DWUXING_" + info.elements[i] + ")" + "\n";
			if(info.elements[i] == "OpenGL" || info.elements[i] == "Vulkan") {
				new_str += "find_package(" + info.elements[i] + " REQUIRED)" + "\n";
				if(info.elements[i] == "OpenGL") {
					transform(info.elements[i].begin(), info.elements[i].end(), info.elements[i].begin(), ::toupper);
					new_str += "link_libraries(${" + info.elements[i] + "_LIBRARIES})" + "\n";
				}
				else if(info.elements[i] == "Vulkan") {
					new_str += "target_link_libraries(${PROJECT_NAME} ${" + info.elements[i] + "_LIBRARIES})" + "\n";
					new_str += "target_include_directories(${PROJECT_NAME} PUBLIC ${" + info.elements[i] + "_INCLUDE_DIRS})\n";
				}
			}
			else {
				new_str += "add_subdirectory(${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + " ${WUXINGENGINE_PATH}/build/3rdPartyLibrary/" + info.elements[i] + ")" + "\n";
				new_str += "link_libraries(" + info.elements[i] + ")" + "\n\n";
			
				if(info.elements[i] == "EnTT") 		new_str += "file(COPY ${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + "/single_include/entt/entt.hpp DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.hpp)" + "\n";
				if(info.elements[i] == "freetype") 	new_str += "file(COPY ${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + "/include/ft2build.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.h)" + "\n";
				if(info.elements[i] == "glad") 		new_str += "file(COPY ${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + "/include/glad/glad.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.h)" + "\n";
				if(info.elements[i] == "glfw") 		new_str += "file(COPY ${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + "/include/GLFW/glfw3.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.h)" + "\n";
				if(info.elements[i] == "glm") 		new_str += "file(COPY ${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + "/glm/glm.hpp DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.hpp)" + "\n";
				if(info.elements[i] == "SDL2") 		new_str += "file(COPY ${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + "/include/SDL.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.h)" + "\n";
				if(info.elements[i] == "tinyxml2")	new_str += "file(COPY ${3RDPARTYLIBRARY_PATH}/" + info.elements[i] + "/tinyxml2.h DESTINATION ${WUXINGENGINE_PATH}/build/win/inc PATTERN *.h)" + "\n";
				}
			break;
		case typeToChangeScript::setAndroidCmakeScript3rdPartyLib:
			new_str += "add_definitions(-DWUXING_" + info.elements[i] + ")" + "\n";
			new_str += "target_link_libraries(${PROJECT_NAME} " + info.elements[i] + ")" + "\n";
			break;
		case typeToChangeScript::setDefineFile:
			new_str += "#ifndef WUXING_" + info.elements[i] + "\n";
			new_str += "#define WUXING_" + info.elements[i] + "\n";
			new_str += "#endif\n";
			break;
		case typeToChangeScript::setCompiler:
			if(info.elements[i] == "gcc" || info.elements[i] == "clang") new_str += "set(CMAKE_C_COMPILER \"" + info.elements[i] + "\")\n";
			if(info.elements[i] == "g++" || info.elements[i] == "clang++") new_str += "set(CMAKE_CXX_COMPILER \"" + info.elements[i] + "\")\n";
			break;
		case typeToChangeScript::print3rdPartyLibMsg:
			new_str += "\t\tcout<<\"" + info.elements[i] + "\"<<endl;" + "\n";
			break;
		case typeToChangeScript::setDefineFileHeaders:
			new_str += "#include \"resource/generated/define/define_" + info.elements[i] +".h\"\n";
			break;
		case typeToChangeScript::linkWindowsCmakeScript3rdPartyLib:
			if(info.elements[i] == "Vulkan") break;
			if(info.elements[i] == "OpenGL") break;
		case typeToChangeScript::linkAndroidCmakeScript3rdPartyLib:
			new_str += "include(${RESOURCE_PATH}/generated/cmake/" + info.elements[i] +".cmake)\n";
			break;
		case typeToChangeScript::setAndroidMirror:
			new_str += "\t\tmaven { setUrl(\"" + info.elements[i] +"\") }\n";
			break;
		default:
			break;
		}
	}
	new_str += info.endWith + "\n";
	for(int i = endLine + 1; i <= FileTXT::getNumOfLine(); i++) { new_str += FileTXT::getStrFromLine(i) + "\n"; }
	
	ofstream file_writter(engine_info.FilePath, std::ios_base::out);
	file_writter<<new_str;
	file_writter.close();
}

void preCompileSet(typeToChangeScript type) {

	PreCompileInfo info;
	WuXing::FILE::FileInfo fileInfo;
	vector<string> elements;

	switch (type)
	{
	case typeToChangeScript::setCompiler:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrCompiler.xml";
		info.aimFilePath = string(_ENGINE_PATH) + "/CMakeLists.txt";
		info.fromWhichLine = "# COMPILER";
		info.endWith = "# compiler";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		info.elements = FileXML::GetEleValue({"WINDOWS"});
		info.type = type;
		setCmakeScriptUsingUserCFG(info);
		break;
	case typeToChangeScript::linkWindowsCmakeListsScript3rdPartyLib:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		info.aimFilePath = string(_ENGINE_PATH) + "/CMakeLists.txt";
		info.fromWhichLine = "# LINK";
		info.endWith = "# link";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		info.elements = FileXML::GetEleValue({"WINDOWS"});
		info.type = type;
		setCmakeScriptUsingUserCFG(info);
		break;

	case typeToChangeScript::setAndroidMirror:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrMirror.xml";
		info.aimFilePath = string(_ANDROID_PATH) + "/settings.gradle";
		info.fromWhichLine = "        // MIRROR";
		info.endWith = "        // mirror";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		info.elements = FileXML::GetEleValue({"ANDROID"});
		info.type = type;
		setCmakeScriptUsingUserCFG(info);
		break;

	case typeToChangeScript::linkAndroidCmakeScript3rdPartyLib:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		info.aimFilePath = string(_ANDROID_PATH) + "/app/src/main/cpp/CMakeLists.txt";
		info.fromWhichLine = "# ADD LIB DEPENDENCIES";
		info.endWith = "# add lib dependencies";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		info.elements = FileXML::GetEleValue({"ANDROID"});
		info.type = type;
		setCmakeScriptUsingUserCFG(info);
		break;

	case typeToChangeScript::linkWindowsCmakeScript3rdPartyLib:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		info.aimFilePath = string(_COMPILELIBS_PATH) + "/CMakeLists.txt";
		info.fromWhichLine = "# 3RDPARTYLIBRARY";
		info.endWith = "# 3rdPartyLibrary";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		info.elements = FileXML::GetEleValue({"WINDOWS"});
		info.type = type;
		setCmakeScriptUsingUserCFG(info);
		break;

	case typeToChangeScript::print3rdPartyLibMsg:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		info.aimFilePath = string(_SOURCE_PATH) + "/core/define.h";
		info.fromWhichLine = "        // DEFINES";
		info.endWith = "        // defines";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		info.elements = FileXML::GetEleValue({"WINDOWS"});
		info.type = type;
		setCmakeScriptUsingUserCFG(info);
		break;

	case typeToChangeScript::setDefineFileHeaders:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		info.aimFilePath = string(_SOURCE_PATH) + "/core/define.h";
		info.fromWhichLine = "// DEFINE HEADERS";
		info.endWith = "// define headers";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		info.elements = FileXML::GetEleValue({"WINDOWS"});
		info.type = type;
		setCmakeScriptUsingUserCFG(info);
		break;

	case typeToChangeScript::setDefineFile:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		elements = FileXML::GetEleValue({"WINDOWS", "ANDROID"});
	
		for(int i = 0; i < elements.size(); i++) {
			if(!elements[i].size()) continue;
			info.aimFilePath = string(_RESOURCE_PATH) + "/generated/define/define_" + elements[i] + ".h";
			info.fromWhichLine = "// DEFINE";
			info.endWith = "// define";
			info.elements.clear();
			info.elements.push_back(elements[i]);
			info.type = type;
			setCmakeScriptUsingUserCFG(info);
		}
		break;

	case typeToChangeScript::setWindowsCmakeScript3rdPartyLib:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		elements = FileXML::GetEleValue({"WINDOWS"});
	
		for(int i = 0; i < elements.size(); i++) {
			if(!elements[i].size()) continue;
			info.aimFilePath = string(_RESOURCE_PATH) + "/generated/cmake/" + elements[i] + ".cmake";
			info.fromWhichLine = "# CMAKE";
			info.endWith = "# cmake";
			info.elements.clear();
			info.elements.push_back(elements[i]);
			info.type = type;
			setCmakeScriptUsingUserCFG(info);
		}
		break;

	case typeToChangeScript::setAndroidCmakeScript3rdPartyLib:
		info.cfgFilePath = string(_RESOURCE_PATH) + "/configs/usrLib.xml";
		fileInfo.FilePath = info.cfgFilePath;
		FileXML::init(fileInfo);
		elements = FileXML::GetEleValue({"ANDROID"});
	
		for(int i = 0; i < elements.size(); i++) {
			if(!elements[i].size()) continue;
			info.aimFilePath = string(_RESOURCE_PATH) + "/generated/cmake/" + elements[i] + ".cmake";
			info.fromWhichLine = "# CMAKE";
			info.endWith = "# cmake";
			info.elements.clear();
			info.elements.push_back(elements[i]);
			info.type = type;
			setCmakeScriptUsingUserCFG(info);
		}
		break;


	default:
		break;
	}
}

int main() {
	preCompileSet(typeToChangeScript::setCompiler);
	preCompileSet(typeToChangeScript::setAndroidMirror);
	preCompileSet(typeToChangeScript::linkAndroidCmakeScript3rdPartyLib);
	preCompileSet(typeToChangeScript::linkWindowsCmakeScript3rdPartyLib);
	preCompileSet(typeToChangeScript::print3rdPartyLibMsg);
	preCompileSet(typeToChangeScript::setDefineFileHeaders);
	preCompileSet(typeToChangeScript::setDefineFile);
	preCompileSet(typeToChangeScript::setWindowsCmakeScript3rdPartyLib);
	preCompileSet(typeToChangeScript::setAndroidCmakeScript3rdPartyLib);
	preCompileSet(typeToChangeScript::linkWindowsCmakeListsScript3rdPartyLib);
/*
*/
	return 0;
}
