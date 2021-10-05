#include <Windows.h>
#include "../../Math/Math.h"
#include <bitset>
#include <vector>
#include <functional>

class Actor { // 1.17.32
public:
    Vector2* bodyRots() {
        return reinterpret_cast<Vector2*>((uintptr_t)(this) + 0x138);
    };

    int* onGround() {
        return reinterpret_cast<int*>((uintptr_t)(this) + 0x1D8);
    };

    float* stepHeight() {
        return reinterpret_cast<float*>((uintptr_t)(this) + 0x238);
    };

    bool* isInWater() {
        return reinterpret_cast<bool*>((uintptr_t)(this) + 0x25D);
    };

    AABB* position() {
        return reinterpret_cast<AABB*>((uintptr_t)(this) + 0x4C0);
    };

    Vector3* velocity() {
        return reinterpret_cast<Vector3*>((uintptr_t)(this) + 0x4F8);
    };

    float* speedInAir() { //Airaccelaration
        return reinterpret_cast<float*>((uintptr_t)(this) + 0x7D8);
    };

    // cVoids
    void teleport(Vector3 v) {
        position()->lower.x = v.x;
        position()->lower.y = v.y;
        position()->lower.z = v.z;

        position()->upper.x = v.x + 0.6f;
        position()->upper.y = v.y + 1.8f;
        position()->upper.z = v.z + 0.6f;
    };

    void teleport(float x, float y, float z) {
        teleport(Vector3(x,y,z));
    };

	void setFieldOfView(float v) {
		*(float*)((uintptr_t)(this) + 0x1140) = v; // 0x1140
	}

    float getSpeed() { //read only 
        return *(float*)((uintptr_t)(this) + 0x840);
    }

    float getFallDistance() { //read only 
        return *(float*)((uintptr_t)(this) + 0x1D4);
    }
};