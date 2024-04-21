#pragma once

#include "pch.h"

using namespace plugin;

class Mod {
public:
    static ISoundEngine* m_SoundEngine;
    static std::map<eWeaponType, ISoundSource*> m_SoundSources;
    static std::string m_Version;

    static bool m_EnableTracers;
    static bool m_EnableMuzzleFlashEffect;

    Mod();

    static void Init();
    static void Update();
    static void Draw();
};
