#pragma once

#include "pch.h"
#include "SoundSourceInfo.h"

using namespace plugin;

class Mod {
public:
    static ISoundEngine* m_SoundEngine;
    static std::map<eWeaponType, SoundSourceInfo*> m_SoundSources;
    static std::string m_Version;

    static bool m_EnableTracers;
    static bool m_EnableTracersForPlayerPed;
    static bool m_EnableMuzzleFlashEffect;

    static float m_TracerSize;
    static float m_TracerLength;
    static float m_TracerCoronaDistanceSpace;

    Mod();

    static void Init();
    static void Update();
    static void Draw();
};
