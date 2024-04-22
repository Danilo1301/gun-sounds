#include "ModConfig.h"

#include "Mod.h"

#include "iniconfig/INIFile.h"

std::string ModConfig::GetFullPath(std::string path)
{
    std::string pluginPath = PLUGIN_PATH("\\");
    std::string finalPath = pluginPath + path;
    return finalPath;
}

void ModConfig::Load()
{
    LoadSettings();
}

void ModConfig::LoadSettings()
{
    auto settingsFileDir = GetFullPath("/settings.ini");

    Log::file << "ModConfig: LoadSettings (settings.ini)" << std::endl;

    INIFile file;
    if (!file.Read(settingsFileDir))
    {
        Log::file << "ModConfig: Error reading settings.ini (Not found)" << std::endl;
        return;
    }

    auto generalSections = file.GetSections("General");
    if (generalSections.size() > 0)
    {
        auto generalSection = generalSections[0];

        Mod::m_EnableTracers = generalSection->GetBool("enable_tracers", Mod::m_EnableTracers);
        Mod::m_EnableTracersForPlayerPed = generalSection->GetBool("enable_tracers_for_player_ped", Mod::m_EnableTracersForPlayerPed);
        Mod::m_EnableMuzzleFlashEffect = generalSection->GetBool("enable_muzzle_flash_effect", Mod::m_EnableMuzzleFlashEffect);

        Mod::m_TracerSize = generalSection->GetFloat("tracer_size", Mod::m_TracerSize);
        Mod::m_TracerLength = generalSection->GetFloat("tracer_length", Mod::m_TracerLength);
        Mod::m_TracerCoronaDistanceSpace = generalSection->GetFloat("tracer_corona_distance_space", Mod::m_TracerCoronaDistanceSpace);
    }

    Log::file << "enable_tracers: " << (Mod::m_EnableTracers ? "true" : "false") << std::endl;
    Log::file << "enable_tracers_for_player_ped: " << (Mod::m_EnableTracersForPlayerPed ? "true" : "false") << std::endl;
    Log::file << "enable_muzzle_flash_effect: " << (Mod::m_EnableMuzzleFlashEffect ? "true" : "false") << std::endl;
    
    Log::file << "tracer_size: " << Mod::m_TracerSize << std::endl;
    Log::file << "tracer_length: " << Mod::m_TracerLength << std::endl;
    Log::file << "tracer_corona_distance_space: " << Mod::m_TracerCoronaDistanceSpace << std::endl;

    Log::file << "ModConfig: Success reading settings.ini" << std::endl;
}