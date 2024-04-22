#include "Mod.h"

#include "CCamera.h"

#include "Peds.h"
#include "TestTracers.h"
#include "ModConfig.h"

ISoundEngine* Mod::m_SoundEngine = NULL;
std::map<eWeaponType, SoundSourceInfo*> Mod::m_SoundSources;
std::string Mod::m_Version = "1.0.0";

bool Mod::m_EnableTracers = true;
bool Mod::m_EnableTracersForPlayerPed = false;
bool Mod::m_EnableMuzzleFlashEffect = true;

float Mod::m_TracerSize = 1.0f;
float Mod::m_TracerLength = 1.0f;
float Mod::m_TracerCoronaDistanceSpace = 0.0004f;

SoundSourceInfo* LoadSoundSource(std::string fileName)
{
    char audioPath[PLUGIN_PATH_MAX];
    sprintf(audioPath, "audios\\%s", fileName.c_str());

    if (!file_exists(audioPath))
    {
        Log::file << "Mod: Sound source '" << fileName << "' could not be found" << std::endl;
        return NULL;
    }

    ISoundSource* source = Mod::m_SoundEngine->addSoundSourceFromFile(PLUGIN_PATH(audioPath));

    SoundSourceInfo* sourceInfo = new SoundSourceInfo{fileName, source};

    Log::file << "Mod: Sound source '" << sourceInfo->name << "' loaded (" << sourceInfo->source << ")" << std::endl;;

    return sourceInfo;
}

Mod::Mod()
{
    Log::Open("GunSounds.log");
    Log::file << "Mod: GunSounds v" << Mod::m_Version << std::endl;

    Events::initGameEvent += Init;
    Events::processScriptsEvent += Update;

    Events::pedCtorEvent += [] (CPed* ped) {
        //PrintMessage("Ped:ctor " + std::to_string(reinterpret_cast<unsigned int>(ped)), 2000);

        if(!Peds::PedExists(ped))
            Peds::CreatePed(ped);
    };

    Events::pedDtorEvent += [] (CPed* ped) {
        //PrintMessage("Ped:dtor " + std::to_string(reinterpret_cast<unsigned int>(ped)), 2000);

        if (Peds::PedExists(ped))
            Peds::RemovePed(ped);
    };

    Events::drawingEvent.before += Draw;
}

void Mod::Init()
{
    Log::file << "Mod: Init" << std::endl;

    ModConfig::Load();

    m_SoundEngine = irrklang::createIrrKlangDevice();

    Log::file << "Mod: Sound engine " << (m_SoundEngine ? "loaded" : "error") << std::endl;;

    Log::file << "Mod: Loading sound sources..." << std::endl;

    m_SoundSources[eWeaponType::WEAPON_MICRO_UZI] = LoadSoundSource("smg\\micro_uzi.wav");
    m_SoundSources[eWeaponType::WEAPON_TEC9] = LoadSoundSource("smg\\tec9.wav");
    m_SoundSources[eWeaponType::WEAPON_MP5] = LoadSoundSource("smg\\mp5.wav");

    m_SoundSources[eWeaponType::WEAPON_AK47] = LoadSoundSource("rifle\\ak47.wav");
    m_SoundSources[eWeaponType::WEAPON_M4] = LoadSoundSource("rifle\\m4.wav");

    m_SoundSources[eWeaponType::WEAPON_PISTOL] = LoadSoundSource("pistol\\pistol.wav");
    m_SoundSources[eWeaponType::WEAPON_DESERT_EAGLE] = LoadSoundSource("pistol\\desert_eagle.wav");

    m_SoundSources[eWeaponType::WEAPON_SAWNOFF] = LoadSoundSource("shotgun\\sawnoff.wav");
    m_SoundSources[eWeaponType::WEAPON_SHOTGUN] = LoadSoundSource("shotgun\\shotgun.wav");
    m_SoundSources[eWeaponType::WEAPON_SPAS12] = LoadSoundSource("shotgun\\spas12.wav");

    m_SoundSources[eWeaponType::WEAPON_PISTOL_SILENCED] = LoadSoundSource("pistol\\pistol_silenced.wav");

    m_SoundSources[eWeaponType::WEAPON_SNIPERRIFLE] = LoadSoundSource("rifle\\sniperrifle.wav");
    m_SoundSources[eWeaponType::WEAPON_COUNTRYRIFLE] = LoadSoundSource("rifle\\countryrifle.wav");

    //unknown weapons
    //m_SoundSources[eWeaponType::WEAPON_ROCKET] = LoadSoundSource("rocket\\rocket.wav");
    //m_SoundSources[eWeaponType::WEAPON_ROCKET_HS] = LoadSoundSource("rocket\\rocket_hs.wav");

    m_SoundSources[eWeaponType::WEAPON_RLAUNCHER] = LoadSoundSource("rocket\\rlauncher.wav"); //rocket launcher
    m_SoundSources[eWeaponType::WEAPON_RLAUNCHER_HS] = LoadSoundSource("rocket\\rlauncher_hs.wav"); //rocket launcher heat seak

}

void Mod::Update()
{
    float distanceMultiplier = 1.0f;

    vec3df camPos(TheCamera.GetPosition().x * distanceMultiplier, TheCamera.GetPosition().z * distanceMultiplier, TheCamera.GetPosition().y * distanceMultiplier);
    m_SoundEngine->setListenerPosition(camPos, vec3df(TheCamera.m_matrix->right.y, TheCamera.m_matrix->up.z, -TheCamera.m_matrix->right.x), vec3df(0, 0, 0), vec3df(0, 1, 0));
    m_SoundEngine->setSoundVolume(1.0f);
    
    for (auto p : Peds::m_Peds)
    {
        auto ped = p.second;
        ped->Update();
    }

    TestTracers::Update();
}

void Mod::Draw()
{
    for (auto p : Peds::m_Peds)
    {
        auto ped = p.second;
        ped->Draw();
    }
}

Mod mod;