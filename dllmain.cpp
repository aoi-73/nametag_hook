#include <windows.h>
#include <iostream>
#include "minhook/include/MinHook.h"

typedef char(__fastcall* tAddNametagEntity)(__int64 a1, __int64 a2);
tAddNametagEntity fnOriginalAddNametagEntity = nullptr;

char __fastcall hkAddNametagEntity(__int64 a1, __int64 a2) {
    if (a1 != 0) {
    }

    fnOriginalAddNametagEntity(a1, a2);

    return 1;
}

// 3. Hilo principal de nuestra DLL
DWORD WINAPI MainThread(LPVOID lpReserved) {
    if (MH_Initialize() != MH_OK) {
        return FALSE;
    }

    DWORD_PTR clientBase = 0;
    while (clientBase == 0) {
        clientBase = (DWORD_PTR)GetModuleHandleA("client.dll");
        Sleep(100);
    }

    DWORD_PTR targetAddress = clientBase + 0x78CC30;

    if (MH_CreateHook((LPVOID)targetAddress, (LPVOID)&hkAddNametagEntity, (LPVOID*)&fnOriginalAddNametagEntity) == MH_OK) {
        MH_EnableHook((LPVOID)targetAddress);
    }

    return TRUE;
}

extern "C" __declspec(dllexport) BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hMod);
            CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
            break;
        case DLL_PROCESS_DETACH:
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            break;
    }
    return TRUE;
}
