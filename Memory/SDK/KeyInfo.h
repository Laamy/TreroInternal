#include <Windows.h>
#include "../../Math/Math.h"
#include <bitset>
#include <vector>
#include <functional>

class KeyInfo { // 1.17.32
public:
    bool* InMenu() {
        return reinterpret_cast<bool*>((uintptr_t)(this) + 0x4B);
    };
    
    bool* isHitting() {
        return reinterpret_cast<bool*>((uintptr_t)(this) + 0x50);
    };
    
    bool* IsPlacing() {
        return reinterpret_cast<bool*>((uintptr_t)(this) + 0x51);
    };
    
    bool* IsPickingNose() {
        return reinterpret_cast<bool*>((uintptr_t)(this) + 0x52);
    };
    
    int* MouseX() {
        return reinterpret_cast<int*>((uintptr_t)(this) + 0x138);
    };
    
    int* MouseY() {
        return reinterpret_cast<int*>((uintptr_t)(this) + 0x13A);
    };
    
    // 0x13E keymap that i DONT NEED!
};