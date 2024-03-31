#pragma once

#include "glad.h"
#include <map>
#include <vector>
#include <iostream>
using namespace std;
namespace WuXing
{
    class BUFFER_POOL {
    private:
        static inline unsigned int m_activeID = 0, m_currentID = 0, m_currentLength = 0;
        // 通过ID寻找首地址和长度
        static inline map<unsigned int, pair<unsigned int, unsigned int>> m_map;
        static inline vector<float> m_data;
    public:
        static unsigned int gen() { return ++m_currentID; }
        static void setActiveID(unsigned int id) { m_activeID = id; }
        static void setData(const vector<float>& data) {
            if(!exist(m_activeID)) {
                m_map.insert({m_activeID, {m_currentLength, data.size()}});
                m_data.insert(m_data.begin() + m_currentLength, data.begin(), data.end());
                m_currentLength += data.size();
            }
            else {
                m_map[m_activeID] = {m_map[m_activeID].first, data.size()};
                for(auto i = m_map[m_activeID].first; i < m_map[m_activeID].first + data.size(); i++) {
                    m_data[i] = data[i - m_map[m_activeID].first];
                }
            }
        }
        static unsigned int getCurrentID() { return m_currentID; }
        static unsigned int getLength(unsigned int id) { return m_map[id].second; }
        static void getData(vector<float>& data) {
            unsigned int addr = m_map.find(m_activeID)->second.first;
            unsigned int leng = m_map.find(m_activeID)->second.second;
            data.resize(leng);
            data.assign(m_data.begin() + addr, m_data.begin() + addr + leng);
        }
        static void del(unsigned int id) { m_map.erase(id); }
        static void sortMemory() {
            /**/
        }
        static bool exist(unsigned int id) { return (m_map.count(id) == 0 ? false:true); }
    };
    class BufferCPU {
    private:
        unsigned int m_id;
    public:
        void gen() {
            m_id = BUFFER_POOL::gen();
        }
        void bind() { BUFFER_POOL::setActiveID(m_id); }
        void setData(const vector<float>& data) {
            bind();
            if((BUFFER_POOL::exist(m_id) && (data.size() > BUFFER_POOL::getLength(m_id))) ||
                ((!BUFFER_POOL::exist(m_id)) && m_id > BUFFER_POOL::getCurrentID())) {
                BUFFER_POOL::del(m_id);
                m_id = BUFFER_POOL::gen();
            }
            bind();
            BUFFER_POOL::setData(data);
            unbind();
        }
        unsigned int getLength() { return BUFFER_POOL::getLength(m_id); }
        void getData(vector<float>& data) {
            bind();
            BUFFER_POOL::getData(data);
            unbind();
        }
        void unbind() { BUFFER_POOL::setActiveID(0); }
        void del() {
            bind();
            BUFFER_POOL::del(m_id);
            unbind();
        }
    };
    class BufferGPU {
        friend class Buffer_VAO;
        friend class Buffer_VBO;
        friend class Buffer_EBO;
        friend class Texture_GPU;
    public:
        enum BufferGPU_Type {
            BufferGPU_TypeVAO,
            BufferGPU_TypeVBO,
            BufferGPU_TypeEBO,
            BufferGPU_TypeTEXTURE,
        };
    private:
        unsigned int m_data;
    public:
        virtual void gen() {};
        virtual void bind() {};
        virtual void unbind() {};
        virtual void del() {};
    };

    class Buffer_VAO : public BufferGPU {
    public:
        void gen() { glGenVertexArrays(1, &m_data); }
        void bind() { glBindVertexArray(m_data); }
        void unbind() { glBindVertexArray(0); }
        void del() { glDeleteVertexArrays(1, &m_data); }
    };

    class Buffer_VBO : public BufferGPU {
    public:
        void gen() { glGenBuffers(1, &m_data); }
        void bind() { glBindBuffer(GL_ARRAY_BUFFER, m_data); }
        template <class S>
        void setData(const S& data) { glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_STATIC_DRAW); }

        template <class S>
        void setSubData(const S& data) { glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data); }

        void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
        void del() { glDeleteBuffers(1, &m_data); }
    };

    class Buffer_EBO : public BufferGPU {
    public:
        void gen() { glGenBuffers(1, &m_data); }
        void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_data); }
        template <class S>
        void setData(const S& data) { glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_STATIC_DRAW); }
        void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
        void del() { glDeleteBuffers(1, &m_data); }
    };
    class Texture_GPU : public BufferGPU {
    public:
        void gen() { glGenTextures(1, &m_data); }
        void bind() { glBindTexture(GL_TEXTURE_2D, m_data); }
        void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
        void del() { glDeleteTextures(1, &m_data); }
    };
} // namespace WuXing
