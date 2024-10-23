// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

bool (__thiscall* MenuLayer_init)(gd::MenuLayer* self);
bool __fastcall MenuLayer_init_H(gd::MenuLayer* self, void* edx) {
    if (!MenuLayer_init(self)) return false;

    auto director = CCDirector::sharedDirector();
    auto label = CCLabelBMFont::create("Hello World!", "bigFont.fnt");
    label->setPosition({ 100.f, 120.f });
    label->setRotation(-0.1f);
    label->setScale(0.8f);
    self->addChild(label);

    return true;
}

DWORD WINAPI my_thread(void* hModule) {

    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

    auto cocos = GetModuleHandleA("libcocos2d.dll");
    auto cocos_ext = GetModuleHandleA("libExtensions.dll");

    MH_CreateHook(
        reinterpret_cast<void*>(0x4af210),
        reinterpret_cast<void**>(&MenuLayer_init_H),
        reinterpret_cast<void**>(&MenuLayer_init));

    /*
    //Hook example
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xOFFSET),
        reinterpret_cast<void**>(&Class_Function_H),
        reinterpret_cast<void**>(&Class_Function));
    */

    MH_EnableHook(MH_ALL_HOOKS);

    return true;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}