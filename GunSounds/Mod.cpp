#include "Mod.h"

#include "CCamera.h"

#include "Peds.h"
#include "TestTracers.h"

ISoundEngine* Mod::m_SoundEngine = NULL;
std::map<eWeaponType, ISoundSource*> Mod::m_SoundSources;

ISoundSource* LoadSoundSource(std::string fileName)
{
    char audioPath[PLUGIN_PATH_MAX];
    sprintf(audioPath, "audios\\%s", fileName.c_str());
    ISoundSource* source = Mod::m_SoundEngine->addSoundSourceFromFile(PLUGIN_PATH(audioPath));

    Log::file << "[Mod] Sound source '" << fileName << "' " << (source ? "loaded" : "error") << std::endl;;

    return source;
}

Mod::Mod()
{
    Log::Open("GunSounds.log");
    Log::file << "Mod" << std::endl;

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
    Log::file << "Init" << std::endl;

    m_SoundEngine = irrklang::createIrrKlangDevice();

    Log::file << "[Mod] Sound engine " << (m_SoundEngine ? "loaded" : "error") << std::endl;;

    m_SoundSources[eWeaponType::WEAPON_M4] = LoadSoundSource("m4.wav");
    m_SoundSources[eWeaponType::WEAPON_AK47] = LoadSoundSource("ak47.wav");
    m_SoundSources[eWeaponType::WEAPON_DESERT_EAGLE] = LoadSoundSource("desert_eagle.wav");
    m_SoundSources[eWeaponType::WEAPON_SNIPERRIFLE] = LoadSoundSource("sniper.wav");
    m_SoundSources[eWeaponType::WEAPON_ROCKET] = LoadSoundSource("rocket.wav");
    m_SoundSources[eWeaponType::WEAPON_ROCKET_HS] = LoadSoundSource("rocket_hs.wav");
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