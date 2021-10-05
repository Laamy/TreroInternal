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
std::map<uint64_t, bool> LKeymap = std::map<uint64_t, bool>();

void keyCallback(uint64_t c, bool v){ // Store key infomation inside our own keymap ;p
    LKeymap[c] = keymap[c];
    keymap[c] = v;
    _key(c, v);
};

bool bhop = false;

void callback(Actor* player){

    if (keymap[(int)'X'] && !LKeymap[(int)'X'])
    {
        bhop = !bhop;
    }

    if (bhop) // internal bhop 1.17.32
    {
        bool wKey = keymap[(int)'W'], sKey = keymap[(int)'S'], aKey = keymap[(int)'A'], dKey = keymap[(int)'D'];
        auto rots = *player->bodyRots();
        auto yaw = rots.y;

        if (wKey)
        {
            if (!aKey && !dKey)
            {
                yaw += 90.0f;
            }
            else if (aKey)
            {
                yaw += 45.0f;
            }
            else if (dKey)
            {
                yaw += 135.0f;
            }
        }
        else if (sKey)
        {
            if (!aKey && !dKey)
            {
                yaw -= 90.0f;
            }
            else if (aKey)
            {
                yaw -= 45.0f;
            }
            else if (dKey)
            {
                yaw -= 135.0f;
            }
        }
        else if (!wKey && !sKey)
        {
            if (dKey)
            {
                yaw += 180.0f;
            }
        }

        if (wKey || aKey || sKey || dKey)
        {
            player->velocity()->x = cos((yaw) * (PI / 180.0f)) * 0.4f;
            if (*player->onGround() == 16777473) player->velocity()->y = 0.2f;
            player->velocity()->z = sin((yaw) * (PI / 180.0f)) * 0.4f;
        }
    }

    if (keymap[0x43])
        player->setFieldOfView(0.2f);
    else player->setFieldOfView(1);

    //_tick(player);
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