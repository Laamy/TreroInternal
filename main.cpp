#include <Windows.h>
#include <MinHook.h>
#include "Math/Math.h"
#include "Memory/Utils.h"
#include "Memory/SDK/Actor.h"
#include "Memory/SDK/KeyInfo.h"
#include <cmath>
#include <map>

// "Here we have a wild yammi in her natural habitat" - Flame

#define PI 3.14159265359 // 3.14159265359

typedef void(__thiscall* tick)(Actor*);
tick _tick;

typedef void(__thiscall* key)(uint64_t, bool);
key _key;

typedef void(__thiscall* keyinfo)(KeyInfo*);
keyinfo _keyinfo;

std::map<uint64_t, bool> keymap = std::map<uint64_t, bool>();

void keyCallback(uint64_t c, bool v){ // Store key infomation inside our own keymap ;p
    keymap[c] = v;
    _key(c, v);
};

void keyinfoCallback(KeyInfo* e){

    *e->IsPlacing() = keymap[0x0];
    _keyinfo(e);
};

bool wasHeld = false;

void callback(Actor* player){

    if (keymap[(int)'F']) // Internal OGMFly
    {
        wasHeld = true;
        auto speedMod = 0.7f;
        
        auto calcyaw = (player->bodyRots()->y + 90) * (PI / 180);

        player->velocity()->x = cos(calcyaw) * speedMod;
        if (keymap[0x10]) {
            player->velocity()->y = -0.075f * speedMod;
        }
        else {
            player->velocity()->y = 0.075f * speedMod;
        }
        player->velocity()->z = sin(calcyaw) * speedMod;
    }
    if (!keymap[(int)'F'] && wasHeld)
    {
        *player->velocity() = Vector3(0,0,0);
        wasHeld = false;
    }

    if (keymap[0x43])
        player->setFieldOfView(0.2f);
    else player->setFieldOfView(1);

    //_tick(player);
};

void init(HMODULE c){
    if(MH_Initialize() == MH_OK){ // *reinterpret_cast<int*>(Mem::findSig("48 8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC 40 55") + 3)

        uintptr_t baseAddr = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");

        // F3 0F 10 81 ? ? ? ? 41 0F 2F 00
        // 48 89 5C 24 08 57 48 83 EC ? 8B 05 ? ? ? ? 8B DA
        // 48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 41 54 41 55  41 56 41 57 48 8B EC 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F0 48 8B ? E8 

        uintptr_t hookAddr = (uintptr_t)(baseAddr + 0x1D6A220); // LocalPlayer hook
        uintptr_t keymapAddr = (uintptr_t)(baseAddr + 0x775230); // Keymap hook
        uintptr_t keyInfoAddr = (uintptr_t)(baseAddr + 0x1533B50); // Keyinfo hook (Hooking a different part to get this keyinfo because idk the fuckinbg arguments and it'll break whatever i hook wiuthout them)

        if(MH_CreateHook((void*)hookAddr, &callback, reinterpret_cast<LPVOID*>(&_tick)) == MH_OK){
            MH_EnableHook((void*)hookAddr);
        };
        if(MH_CreateHook((void*)keymapAddr, &keyCallback, reinterpret_cast<LPVOID*>(&_key)) == MH_OK){
            MH_EnableHook((void*)keymapAddr);
        };
        //if(MH_CreateHook((void*)keyInfoAddr, &keyinfoCallback, reinterpret_cast<LPVOID*>(&_keyinfo)) == MH_OK){
        //   MH_EnableHook((void*)keyInfoAddr);
        //};
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