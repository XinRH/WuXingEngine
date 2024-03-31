#pragma once

#include "function/physics.h"
#include "function/renderHardwareInterface.h"

namespace WuXing
{

class RenderPass;
class RENDERPASS {
    public:
    static inline RenderPass* renderPass_PhyWorld;
    static inline RenderPass* renderPass_Text;
};
class RenderPass {
    friend class RenderPass_PhyWorld;
    friend class RenderPass_Text;

    public:
    virtual void init() {};
    virtual void prepareRenderData() {};
    virtual void swapBuffer() {};
    virtual void draw() {};

    private:
    map<string, BufferGPU*> m_vaoMap;
    map<string, BufferGPU*> m_vboMap;
    map<string, BufferGPU*> m_eboMap;
    map<string, BufferGPU*> m_TextureMap;

    public:
    virtual void clear() {
        std::map<string, BufferGPU*>::iterator itor;
        for (itor = m_vaoMap.begin(); itor != m_vaoMap.end();) {
            itor->second->del();
            delete itor->second;
            itor->second = nullptr;
            m_vaoMap.erase(itor++);
        }
        for (itor = m_vboMap.begin(); itor != m_vboMap.end();) {
            itor->second->del();
            delete itor->second;
            itor->second = nullptr;
            m_vboMap.erase(itor++);
        }
        for (itor = m_eboMap.begin(); itor != m_eboMap.end();) {
            itor->second->del();
            delete itor->second;
            itor->second = nullptr;
            m_eboMap.erase(itor++);
        }
        for (itor = m_TextureMap.begin(); itor != m_TextureMap.end();) {
            itor->second->del();
            delete itor->second;
            itor->second = nullptr;
            m_TextureMap.erase(itor++);
        }
    }

    private:
    void genBufferGPU(BufferGPU::BufferGPU_Type type, string name) {
        BufferGPU* buf;
        switch (type)
        {
        case BufferGPU::BufferGPU_Type::BufferGPU_TypeVAO:
            buf = new Buffer_VAO;
            buf->gen();
            buf->bind();
            m_vaoMap[name] = buf;
            break;
            
        case BufferGPU::BufferGPU_Type::BufferGPU_TypeVBO:
            buf = new Buffer_VBO;
            buf->gen();
            buf->bind();
            m_vboMap[name] = buf;
            break;

        case BufferGPU::BufferGPU_Type::BufferGPU_TypeEBO:
            buf = new Buffer_EBO;
            buf->gen();
            buf->bind();
            m_eboMap[name] = buf;
            break;

        case BufferGPU::BufferGPU_Type::BufferGPU_TypeTEXTURE:
            buf = new Texture_GPU;
            buf->gen();
            buf->bind();
            m_TextureMap[name] = buf;
            break;
    
        default:
            break;
        }
    }
    void setVBO(string name, const vector<float>& vertices) { dynamic_cast<Buffer_VBO*>(m_vboMap[name])->setData(vertices); }
    template <class T>
    void setSubVBO(string name, const T& vertices) { dynamic_cast<Buffer_VBO*>(m_vboMap[name])->setSubData(vertices); }
};

class RenderPass_PhyWorld : public RenderPass {    
    private:
    Geometry m_renderGeom;

    public:
    void init() {
        // genBuffer
        genBufferGPU(BufferGPU::BufferGPU_Type::BufferGPU_TypeVAO, "phyWorld");
        genBufferGPU(BufferGPU::BufferGPU_Type::BufferGPU_TypeVBO, "phyWorld");
        genBufferGPU(BufferGPU::BufferGPU_Type::BufferGPU_TypeEBO, "phyWorld");
        
        // init Geom settings
        m_renderGeom.init();
        m_renderGeom.setAttr(GEOMETRY::GeometryType::Rectangles, 3);
        
        // init EBO
        vector<int> indices;
        vector<int> m_indices;
        switch (m_renderGeom.getType())
        {
        case GEOMETRY::GeometryType::Points:
            for(int i = 0; i < PARTICLES::getNum(); i++) { m_indices.push_back(i); }
            break;
        case GEOMETRY::GeometryType::Triangles:
            break;
        case GEOMETRY::GeometryType::Rectangles:
            for(int i = 0; i < (4-2); i++) {
                indices.push_back(0);
                for(int j = (i+1); j < (4+i-1); j++) {
                    indices.push_back(j);
                }
            }
            for(int i = 0; i < PARTICLES::getNum(); i++) {
                m_indices.insert(m_indices.end(), indices.begin(), indices.end());
                for(int j = 0; j < 6; j++) { indices[j] += 4; }
            }
            break;
        default:
            break;
        };
        setEBO("phyWorld", m_indices);
        
        switch (getRenderGeom().getType())
        {
        case GEOMETRY::GeometryType::Points:
            RHI::renderRHI->setPointSize(10);
            break;
        case GEOMETRY::GeometryType::Rectangles:
            break;
        default:
            break;
        }
        // vao setting
        // location = 0
        // getDim()个一组，都由float组成
        RenderPipeline::VertexAttribPointer(0, getRenderGeom().getDim());
        // pipeline init setting
        // enable (location = 0) in glsl
        RenderPipeline::EnableVertexAttribArray(0);
    }
    void prepareRenderData() {
        // phyWorld
        float thickness = 0.05f;
        vector<float> thick;
        for(int i = 0; i < PARTICLES::getDim(); i++) {
            thick.push_back(thickness);
        }
        vector<float> tempParticlePosData;
        vector<float> rectRenderData;
        PARTICLES::getPosData(tempParticlePosData);
        for(int i = 0; i < tempParticlePosData.size(); i += PARTICLES::getDim()) {
            // 粒子系统中每个待渲染的粒子坐标
            vector<float> pos;
            for(int j = 0; j < PARTICLES::getDim(); j++) {
                pos.push_back(tempParticlePosData[i + j]);
            }
            // 每个粒子位置处渲染一个几何体，储存其所有坐标
            vector<float> geomPos;
            switch (m_renderGeom.getType())
            {
                case GEOMETRY::GeometryType::Points:
                    for(int j = 0; j < m_renderGeom.getDim(); j++) {
                        if(j > PARTICLES::getDim() - 1) rectRenderData.push_back(0.f);
                        else { rectRenderData.push_back(pos[j]); }
                    }
                    break;
                case GEOMETRY::GeometryType::Triangles:
                    break;
                case GEOMETRY::GeometryType::Rectangles:
                    for(int j = 0; j < m_renderGeom.getDim(); j++) {
                        if(j > PARTICLES::getDim() - 1) rectRenderData.push_back(0.f);
                        else {
                            if(j == 0) rectRenderData.push_back(pos[j] - thick[j]/2);
                            if(j == 1) rectRenderData.push_back(pos[j] - thick[j]/2);
                            if(j == 2) rectRenderData.push_back(pos[j] - thick[j]/2);
                        }
                    }
                    for(int j = 0; j < m_renderGeom.getDim(); j++) {
                        if(j > PARTICLES::getDim() - 1) rectRenderData.push_back(0.f);
                        else {
                            if(j == 0) rectRenderData.push_back(pos[j] + thick[j]/2);
                            if(j == 1) rectRenderData.push_back(pos[j] - thick[j]/2);
                            if(j == 2) rectRenderData.push_back(pos[j] - thick[j]/2);
                        }
                    }
                    for(int j = 0; j < m_renderGeom.getDim(); j++) {
                        if(j > PARTICLES::getDim() - 1) rectRenderData.push_back(0.f);
                        else {
                            if(j == 0) rectRenderData.push_back(pos[j] + thick[j]/2);
                            if(j == 1) rectRenderData.push_back(pos[j] + thick[j]/2);
                            if(j == 2) rectRenderData.push_back(pos[j] - thick[j]/2);
                        }
                    }
                    for(int j = 0; j < m_renderGeom.getDim(); j++) {
                        if(j > PARTICLES::getDim() - 1) rectRenderData.push_back(0.f);
                        else {
                            if(j == 0) rectRenderData.push_back(pos[j] - thick[j]/2);
                            if(j == 1) rectRenderData.push_back(pos[j] + thick[j]/2);
                            if(j == 2) rectRenderData.push_back(pos[j] - thick[j]/2);
                        }
                    }
                    break;
            
                default:
                    break;
            }
        }
        m_renderGeom.setData(rectRenderData);
    }
    void swapBuffer() {
        RenderPipeline::use("phyWorld");
        m_vaoMap["phyWorld"]->bind();
        m_vboMap["phyWorld"]->bind();

        vector<float> data;
        getData(data);
        setVBO("phyWorld", data);
                    
        RenderPipeline::setUniformMatrix4fv("phyWorld", "view", CAMERA3D::getViewMat());
        RenderPipeline::setUniformMatrix4fv("phyWorld", "projection", CAMERA3D::getProjectionMat());
        RenderPipeline::setUniformMatrix4fv("phyWorld", "transform", CAMERA3D::getTransformMat());

        m_vboMap["phyWorld"]->unbind();
        m_vaoMap["phyWorld"]->unbind();
    }
    
    void getData(vector<float>& data) { m_renderGeom.getBuffer().getData(data); }
    Geometry getRenderGeom() { return m_renderGeom; }
    void setEBO(string name, const vector<int>& indices) { dynamic_cast<Buffer_EBO*>(m_eboMap[name])->setData(indices); }

    void draw() {
        
        RenderPipeline::use("phyWorld");
        m_vaoMap["phyWorld"]->bind();
        m_vboMap["phyWorld"]->bind();

        switch (getRenderGeom().getType())
        {
        case GEOMETRY::GeometryType::Points:
            RHI::renderRHI->drawElements(GEOMETRY::GeometryType::Points, getRenderGeom().getNum());
            break;
        case GEOMETRY::GeometryType::Rectangles:
            RHI::renderRHI->drawElements(GEOMETRY::GeometryType::Rectangles, getRenderGeom().getNum());
            break;
        default:
            break;
        }
        m_vboMap["phyWorld"]->unbind();
        m_vaoMap["phyWorld"]->unbind();
    }
    void clear() { m_renderGeom.clear(); }
    
};

class RenderPass_Text : public RenderPass { 
    public:
    void init() {
        RHI::renderRHI->initTextureOptions();
        
        // genBuffer
        RenderPipeline::use("text");
        genBufferGPU(BufferGPU::BufferGPU_Type::BufferGPU_TypeVAO, "text");
        genBufferGPU(BufferGPU::BufferGPU_Type::BufferGPU_TypeVBO, "text");
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

        RenderPipeline::VertexAttribPointer(0,4);
        RenderPipeline::EnableVertexAttribArray(0);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FileFont::loadGlyph(c))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            // generate texture
            string name;
            name = c;
            genBufferGPU(BufferGPU::BufferGPU_TypeTEXTURE, name);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                FileFont::getFace()->glyph->bitmap.width,
                FileFont::getFace()->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                FileFont::getFace()->glyph->bitmap.buffer
            );
            // set texture options
            RHI::renderRHI->setTextureOptions();
            // now store character for later use
            FileFont::Character character = {
                glm::ivec2(FileFont::getFace()->glyph->bitmap.width, FileFont::getFace()->glyph->bitmap.rows),
                glm::ivec2(FileFont::getFace()->glyph->bitmap_left, FileFont::getFace()->glyph->bitmap_top),
                static_cast<unsigned int>(FileFont::getFace()->glyph->advance.x)
            };
            FileFont::getCharacters().insert(std::pair<char, FileFont::Character>(c, character));
        }
    }
    
    void draw() {
        RenderPipeline::use("text");
        m_vaoMap["text"]->bind();
        m_vboMap["text"]->bind();
        RenderText("FPS: " + to_string(1.0f / TIME::deltaTime()), -1.f, -1.f, 0.002f, glm::vec3(0.1, 0.2f, 0.3f));
        m_vboMap["text"]->unbind();
        m_vaoMap["text"]->unbind();
    }
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
    {
        // activate corresponding render state
        RenderPipeline::setUniform3fv("text", "textColor", color);
        RenderPipeline::ActiveTexture(0);
    
        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            WuXing::FileFont::Character ch = WuXing::FileFont::getCharacters()[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
    
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
    
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            string name;
            name = *c;
            m_TextureMap[name]->bind();

            // update content of VBO memory
            //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
            setSubVBO("text", vertices);            

            // render quad
            RHI::renderRHI->drawArrays(GEOMETRY::GeometryType::Triangles, 2);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
    }
    
};


} // namespace WuXing
