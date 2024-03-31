#pragma once

#include "core/buffer.h"
#include <vector>
using namespace std;

namespace WuXing
{
    class GEOMETRY {
        public:
        enum GeometryType {
            // 点
            Points,
            Vectors,
            // 线
            // Lines,
            // 面
            Triangles,
            Rectangles,
            // 体
            // 棱锥
            RectangularPyramids,
            // 多面体
            
        };
    };
    class Geometry
    {
    private:
        BufferCPU m_buffer;
        GEOMETRY::GeometryType m_type;
        int m_dim = 0, m_num = 0;
    public:
        void init() {
            m_buffer.gen();
            m_buffer.bind();
        }
        void setAttr(GEOMETRY::GeometryType type, int dim) {
            m_type = type;
            m_dim = dim;
        }
        void setData(const vector<float>& points) {
            m_buffer.bind();
            m_buffer.setData(points);
            m_buffer.unbind();

            if(!m_dim) return;
            switch (m_type)
            {
            case GEOMETRY::GeometryType::Points:
            case GEOMETRY::GeometryType::Vectors:
                m_num = m_buffer.getLength()/m_dim;
                break;
            case GEOMETRY::GeometryType::Triangles:
                m_num = m_buffer.getLength()/(m_dim*3);
                break;
            case GEOMETRY::GeometryType::Rectangles:
                m_num = m_buffer.getLength()/(m_dim*4);
                break;    
            default:
                break;
            }
        }
        int getNum() { return m_num; }
        int getDim() { return m_dim; }
        GEOMETRY::GeometryType getType() { return m_type; }
        BufferCPU getBuffer() { return m_buffer; }
        void clear() { m_buffer.del(); }
    };
} // namespace WuXing
