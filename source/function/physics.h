#pragma once
#include "core/geometry.h"

namespace WuXing
{
    class PHYSICS {
    public:
        static inline float dt = 0.001f;
    };

    class PARTICLES {
    private:
        static inline Geometry position, velocity;
    public:
        static void init(int dim) {
            position.init();
            position.setAttr(GEOMETRY::GeometryType::Points, dim);
            velocity.init();
            velocity.setAttr(GEOMETRY::GeometryType::Vectors, dim);
        }
        static void tick() {
            vector<float> posData, vecData;
            getPosData(posData);
            getVecData(vecData);
            for(int i = 0; i < posData.size(); i++) {
                posData[i] += vecData[i] * PHYSICS::dt;
                if(posData[i] < -1 || posData[i] > 1) { vecData[i] *= -1; }
            }
            setPosData(posData);
            setVecData(vecData);
        }
        static void setPosData(const vector<float>& data) { position.setData(data); }
        static void setVecData(const vector<float>& data) { velocity.setData(data); }
        static void getPosData(vector<float>& data) { position.getBuffer().getData(data); }
        static void getVecData(vector<float>& data) { velocity.getBuffer().getData(data); }
        static int getDim() { return position.getDim(); }
        static void clear() {
            position.clear();
            velocity.clear();
        }
        static int getNum() { return position.getNum(); }
    };
} // namespace WuXing
