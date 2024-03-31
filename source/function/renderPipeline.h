#pragma once
#include "glad.h"
#include <vector>
#include <string>
#include <iostream>

#ifdef WUXING_glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace WuXing
{
    class RenderPipeline {
        public:
        enum shaderType {
            vertexShader,
            fragmentShader
        };
        struct shaderSourceInfo
        {
            shaderType type;
            std::string source;
        };

        private:
        static inline std::map<string, unsigned int> m_shaderProgramMap;
        public:
        static void init() {}
        static void add(string programName, std::vector<shaderSourceInfo> infos) {
            const char* vertexShaderSource;
            const char* fragmentShaderSource;
            for(int i = 0;i<infos.size();i++) {
                switch (infos[i].type)
                {
                case shaderType::vertexShader:
                    vertexShaderSource = infos[i].source.c_str();
                    //std::cout<<"[vShaderInfo.source] is "<<std::endl<<infos[i].source<<std::endl<<std::endl;
                    break;
                case shaderType::fragmentShader:
                    fragmentShaderSource = infos[i].source.c_str();
                    //std::cout<<"[fShaderInfo.source] is "<<std::endl<<infos[i].source<<std::endl;
                    break;
                default:
                    break;
                }
            }

            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);

            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            
            unsigned int shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            glUseProgram(shaderProgram);
            m_shaderProgramMap.insert(pair<string, unsigned int>(programName, shaderProgram));
        }
        static void use(string programName) { glUseProgram(m_shaderProgramMap[programName]); }
        static unsigned int getShaderProgram(string programName) {
            map<string, unsigned int>::iterator itor;
            for(itor = m_shaderProgramMap.begin(); itor != m_shaderProgramMap.end(); ++itor) {
                if(itor->first == programName) return itor->second;
            }
            return 0;
        }
        
        static void setUniform1i(string programName, string uniformName, int i0)                            { glUniform1i(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), i0); }
        static void setUniform2i(string programName, string uniformName, int i0, int i1)                    { glUniform2i(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), i0, i1); }
        static void setUniform3i(string programName, string uniformName, int i0, int i1, int i2)            { glUniform3i(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), i0, i1, i2); }
        static void setUniform4i(string programName, string uniformName, int i0, int i1, int i2, int i3)    { glUniform4i(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), i0, i1, i2, i3); }
        
        static void setUniform1f(string programName, string uniformName, float f0)                                  { glUniform1f(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), f0); }
        static void setUniform2f(string programName, string uniformName, float f0, float f1)                        { glUniform2f(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), f0, f1); }
        static void setUniform3f(string programName, string uniformName, float f0, float f1, float f2)              { glUniform3f(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), f0, f1, f2); }
        static void setUniform4f(string programName, string uniformName, float f0, float f1, float f2, float f3)    { glUniform4f(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), f0, f1, f2, f3); }
        
        static void setUniform1d(string programName, string uniformName, double d0)                                     { glUniform1d(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), d0); }
        static void setUniform2d(string programName, string uniformName, double d0, double d1)                          { glUniform2d(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), d0, d1); }
        static void setUniform3d(string programName, string uniformName, double d0, double d1, double d2)               { glUniform3d(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), d0, d1, d2); }
        static void setUniform4d(string programName, string uniformName, double d0, double d1, double d2, double d3)    { glUniform4d(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), d0, d1, d2, d3); }
        
        static void setUniform1ui(string programName, string uniformName, unsigned int ui0)                                                         { glUniform1ui(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), ui0); }
        static void setUniform2ui(string programName, string uniformName, unsigned int ui0, unsigned int ui1)                                       { glUniform2ui(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), ui0, ui1); }
        static void setUniform3ui(string programName, string uniformName, unsigned int ui0, unsigned int ui1, unsigned int ui2)                     { glUniform3ui(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), ui0, ui1, ui2); }
        static void setUniform4ui(string programName, string uniformName, unsigned int ui0, unsigned int ui1, unsigned int ui2, unsigned int ui3)   { glUniform4ui(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), ui0, ui1, ui2, ui3); }
        
        static void setUniform1fv(string programName, string uniformName, glm::vec1 data) { glUniform1fv(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), 1, (const float*)glm::value_ptr(data)); }
        static void setUniform2fv(string programName, string uniformName, glm::vec2 data) { glUniform2fv(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), 2, (const float*)glm::value_ptr(data)); }
        static void setUniform3fv(string programName, string uniformName, glm::vec3 data) { glUniform3fv(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), 3, (const float*)glm::value_ptr(data)); }
        static void setUniform4fv(string programName, string uniformName, glm::vec4 data) { glUniform4fv(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), 4, (const float*)glm::value_ptr(data)); }
        
        static void setUniformMatrix2fv(string programName, string uniformName, glm::mat2 data) { glUniformMatrix2fv(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), 1, GL_FALSE, (const float*)glm::value_ptr(data)); }
        static void setUniformMatrix3fv(string programName, string uniformName, glm::mat3 data) { glUniformMatrix3fv(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), 1, GL_FALSE, (const float*)glm::value_ptr(data)); }
        static void setUniformMatrix4fv(string programName, string uniformName, glm::mat4 data) { glUniformMatrix4fv(glGetUniformLocation(getShaderProgram(programName) , uniformName.c_str()), 1, GL_FALSE, (const float*)glm::value_ptr(data)); }
        
        static void EnableVertexAttribArray(int i) { glEnableVertexAttribArray(0); }
        static void VertexAttribPointer(int location, int eachSize) { glVertexAttribPointer(location, eachSize, GL_FLOAT, GL_FALSE, eachSize * sizeof(float), (void*)0); }

        static void ActiveTexture(int n) { glActiveTexture(GL_TEXTURE0 + n); }

        static void clear() {
            map<string, unsigned int>::iterator itor;
            for(itor = m_shaderProgramMap.begin(); itor != m_shaderProgramMap.end(); ++itor) {
                glDeleteProgram(itor->second);
                m_shaderProgramMap.erase(itor);
            }
        }
    };
} // namespace WuXing
