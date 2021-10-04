#include <Windows.h>
#include <MinHook.h>
#include "Math/Math.h"
#include "Memory/SDK/Actor.h"
#include <cmath>
#include <map>

// "Here we have a wild yammi in her natural habitat" - Flame

#define PI 3.14159265359 // 3.14159265359

typedef void(__thiscall* tick)(Actor*);
tick _tick;

typedef void(__thiscall* key)(uint64_t, bool);
key _key;

std::map<uint64_t, bool> keymap = std::map<uint64_t, bool>();

void keyCallback(uint64_t c, bool v){ // Store key infomation inside our own keymap ;p
    keymap[c] = v;
    _key(c, v);
};

void callback(Actor* player){

    if (keymap[0x43]) { // (int)'C'
        player->setFieldOfView(0.1f);
    }
    else {
        player->setFieldOfView(1);
    }

    _tick(player);
};

void init(HMODULE c){
    if(MH_Initialize() == MH_OK){ // *reinterpret_cast<int*>(Mem::findSig("48 8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC 40 55") + 3)
        uintptr_t baseAddr = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
        uintptr_t hookAddr = (uintptr_t)(baseAddr + 0x1D6A220); // Minecraft.Windows.exe+1D6A220 
        uintptr_t keymapAddr = (uintptr_t)(baseAddr + 0x775230); //Minecraft.Windows.exe+775230 

        if(MH_CreateHook((void*)hookAddr, &callback, reinterpret_cast<LPVOID*>(&_tick)) == MH_OK){
            MH_EnableHook((void*)hookAddr);
        };
        if(MH_CreateHook((void*)keymapAddr, &keyCallback, reinterpret_cast<LPVOID*>(&_key)) == MH_OK){
            MH_EnableHook((void*)keymapAddr);
        };
    };
};

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpReserved){
    switch(fdwReason){
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, hInstance, 0, 0);
        break;
    };
    return TRUE;
};

/*

if ((keymap[87] | keymap[0x41] | keymap[0x53] | keymap[0x44])){
           float yaw = player->bodyRots()->y; // yaw
            
            if (keymap[87]){
                if (!keymap[0x41] && !keymap[0x53]){
                    yaw += 90;
                }
                if (keymap[0x41]){
                    yaw += 45;
                }
                else if (keymap[0x44]){
                    yaw += 135;
                }
            }
            else if (keymap[0x53]){
                if (!keymap[0x41] && !keymap[0x53]){
                    yaw -= 90;
                }
                if (keymap[0x41]){
                    yaw -= 45;
                }
                else if (keymap[0x44]){
                    yaw -= 135;
                }
            }
            else if (!keymap[87] && !keymap[0x53]){
                if (!keymap[0x41] && keymap[0x44]){
                    yaw += 180;
                }
            }
            
            if (player->touchingBlock()){
                player->velocity()->y = 0.3f;
            }
            auto speed = 3.0f;

            *player->velocity() = Vector3(cos((yaw) * (PI / 180.f)) * speed, player->velocity()->y, sin((yaw) * (PI / 180.f)) * speed);
        }

*/