#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


#ifdef WUXING_tinyxml2
#include <tinyxml2.h>
using namespace tinyxml2;
#endif


#ifndef _PRECOMPILE_ONLY
#include "core/define.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

using namespace std;

#ifndef _PRECOMPILE_PATH
#define _PRECOMPILE_PATH
#endif

#ifndef _RESOURCE_PATH
#define _RESOURCE_PATH
#endif

#ifndef _ANDROID_PATH
#define _ANDROID_PATH
#endif

namespace WuXing
{
    class FileTXT;
    class FileXML;
    class FILE {
        public:
        struct FileInfo {
            string FilePath;
        };
    };
    
    class FileTXT {
        private:
        static inline vector<string> m_TXT;
        static inline string m_Content;
        static inline string m_FilePath;

        public:
        static void init(FILE::FileInfo info) {
            clear();
            string tmp_str;
            try {
                ifstream f;
                f.open(info.FilePath.c_str());
                //while(getline(f, tmp_str)) { m_TXT.push_back(tmp_str); }

                stringstream strStream;
                strStream << f.rdbuf();
                m_Content = strStream.str();

                f.close();
            	string tmp_str;
            	while(getline(strStream, tmp_str, '\n')) {
                    m_TXT.push_back(tmp_str);
                }
            }
            catch (ifstream::failure& e) {
                cout<<"ERROR! FILE_NOT SUCCESSFULLY_READ: "<<e.what()<<endl;
            }
        }
        static const string& getFilePath() { return m_FilePath; }
        static const string& getStrFromLine(int i) {
            if(1 <= i && i <= m_TXT.size()) return m_TXT[i-1];
            else throw;
        }
        static const string& getContent() { return m_Content; }
        static int getNumOfLine() { return m_TXT.size(); }
        static void clear() {
            m_TXT.clear();
            m_Content.clear();
            m_FilePath.clear();
        }
    };
    
    #ifdef WUXING_tinyxml2
    class FileXML {
        private:
        static inline tinyxml2::XMLDocument m_XML;
        static inline string m_FilePath;

        public:
        static void init(FILE::FileInfo info) {
            m_FilePath = info.FilePath;
            tinyxml2::XMLError error = m_XML.LoadFile(m_FilePath.c_str());
            if(error) { cout<<""<<m_XML.ErrorName()<<endl; }
        }
        static string getFilePath() { return m_FilePath; }
        
        static std::string findXML_Attribute(const std::initializer_list<std::string>& list) {
            vector<string> arr = list;
            tinyxml2::XMLElement* ele = m_XML.FirstChildElement("ROOT");
            for(auto i = 0; i < arr.size()-1; i++) {
                ele = ele->FirstChildElement(arr[i].c_str());
            }
            return ele->Attribute(arr[arr.size()-1].c_str());
        }
        static void clear() {
            m_FilePath.clear();
            m_XML.Clear();
        }
        
        // 获取根节点ROOT下某节点所有子节点的所有属性值
        static vector<string> GetEleValue(const initializer_list<string>& list, XMLElement* element = nullptr)
        {
            vector<string> str;
            vector<string> nodeNameVec = list;
            if(element == nullptr) element = m_XML.RootElement();

            for (tinyxml2::XMLElement *elem = element->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
                if(!nodeNameVec.size()) {
                    for(const XMLAttribute* attr = elem->FirstAttribute(); attr != NULL; attr = attr->Next()) {
                        str.push_back(attr->Value());
                    }
                    vector<string> temp_str = GetEleValue({}, elem);
                    str.insert(str.end(), temp_str.begin(), temp_str.end());
                }
                for(int i = 0; i < nodeNameVec.size(); i++) {
                    if(nodeNameVec[i] != elem->Name()) continue;
                    for(const XMLAttribute* attr = elem->FirstAttribute(); attr != NULL; attr = attr->Next()) {
                        str.push_back(attr->Value());
                    }
                    vector<string> temp_str = GetEleValue({}, elem);
                    str.insert(str.end(), temp_str.begin(), temp_str.end());
                }
            }
            return str;
        }
    };
    #endif

    #ifndef _PRECOMPILE_ONLY
    class FileFont {
    public:
        /// Holds all state information relevant to a character as loaded using FreeType
        struct Character {
            glm::ivec2   Size;      // Size of glyph
            glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
            unsigned int Advance;   // Horizontal offset to advance to next glyph
        };
    private:
        static inline std::map<GLchar, Character> Characters;
        static inline FT_Library m_ft;
        static inline FT_Face m_face;

    public:
        static FT_Face getFace() { return m_face; }
        static std::map<GLchar, Character>& getCharacters() { return Characters; }
        static void init() {
            if (FT_Init_FreeType(&m_ft)) {
                std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
                return;
            }
            std::string font_name = string(_RESOURCE_PATH) + "/fonts/ariblk.ttf";
            if (font_name.empty()) {
                std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
                return;
            }
            if (FT_New_Face(m_ft, font_name.c_str(), 0, &m_face)) {
                std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
                return;
            }
            else { FT_Set_Pixel_Sizes(m_face, 0, 48); }
        }
        static bool loadGlyph(unsigned char c) {
            return FT_Load_Char(FileFont::getFace(), c, FT_LOAD_RENDER);
        }
        static void clear() {
            FT_Done_Face(m_face);
            FT_Done_FreeType(m_ft);
        }
    };
    #endif
} // namespace WuXing


