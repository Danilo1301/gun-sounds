#include "Ped.h"
#include "Mod.h"
#include "TestTracers.h"

#include "CShadows.h"
#include "CTimer.h"
#include "CCoronas.h"

#include "CGeneral.h"
#include "CGame.h"

/*
GTA SA 1.0.0.0 US
Unhandled exception at 0x005334FE in gta_sa.exe (+0x1334fe): 0xC0000005: Access violation reading location 0x00000018.
    Register dump:
        EAX: 0x00B74240  EBX: 0x1965C0A8  ECX: 0x00000018  EDX: 0x00B2DCB4
        EDI: 0x00000000  ESI: 0x00B6F9CC  EBP: 0x00C6A224  EIP: 0x005334FE
        ESP: 0x0177F5A0  EFL: 0x00210202  CS: 0x00000023   SS: 0x0000002B
        GS: 0x0000002B   FS: 0x00000053   ES: 0x0000002B   DS: 0x0000002B

    Stack dump:
        0x0177F5A0:  38EE9940 38EE9CA0 0177F5BC 05932373 01AB0000 022DF0B4
        0x0177F5B8:  0071120D 0177F5D0 00000018 00C6A5A4 01C6BCA0 00000001
        0x0177F5D0:  1768ADA8 4402FC8C BDE16ED3 00711825 00B6F9CC 00000183
        0x0177F5E8:  0177F66C 00000001 00000000 00000000 40800000 3F800000
        0x0177F600:  42200000 00000000 005D805C 00711DD2 00B6F9CC 00000000
        0x0177F618:  1BE6EC1C 0177F66C 0053C1B0 00B6F9CC 01B21C20 FFFFFFFF
        0x0177F630:  FFFFFFFF BFA0E7F5 3FA0E7F5 BFC00000 03C9DAAE 01B21C20
        0x0177F648:  0177F704 FFFFFFFF 2A13CBC0 0177F644 0177EF64 0177F6C0
        0x0177F660:  03CB5D64 03CDA678 00000002 0177F684 0053E986 7C56F75A
        0x0177F678:  0177F704 7C56FC50 00000001 0177F6CC 7C56B8E9 0177F6D8
        base: 0x01580000   top: 0x0177F5A0   bottom: 0x01780000

    Backtrace (may be wrong):
        =>0x005334FE TransformFromObjectSpace__7CEntityRC7CVector+0xe in gta_sa.exe (+0x1334fe) (0x0177F5A8)
          0x05932373 in iii.vc.sa.limitadjuster.asi (+0x42373) (0x0177F5D8)
          0x00711825 CStencilShadowObjects::RegisterStencilShadows+0xc5 in gta_sa.exe (+0x311825) (0x0177F604)
          0x005D805C sub_5D8050+0xc in gta_sa.exe (+0x1d805c)



*/

CMatrix CloneCMatrix(CMatrix matrix) {
    RwMatrix* rwm = new RwMatrix();
    matrix.CopyToRwMatrix(rwm);
    return CMatrix(rwm, true);
}

CVector TransformFromGunMuzzlePosition(CPed* ped, CVector offset)
{
    if (ped->m_pWeaponObject > 0)
    {
        CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
        CWeaponInfo* weaponInfo = CWeaponInfo::GetWeaponInfo(weapon.m_eWeaponType, ped->m_nWeaponSkill);

        RwV3d of = {
            weaponInfo->m_vecFireOffset.x + offset.x,
            weaponInfo->m_vecFireOffset.y + offset.y,
            weaponInfo->m_vecFireOffset.z + offset.z
        };

        ped->GetTransformedBonePosition(of, BONE_RIGHTWRIST, false);

        return CVector(of.x, of.y, of.z);
    }

    return ped->GetPosition();
}

Ped::Ped(CPed* ped)
{
	m_Ped = ped;
    m_TracerColor = CRGBA(
        (unsigned char)floor(CGeneral::GetRandomNumberInRange(30, 255)),
        (unsigned char)floor(CGeneral::GetRandomNumberInRange(30, 255)),
        (unsigned char)floor(CGeneral::GetRandomNumberInRange(30, 255))
    );
}

void Ped::Update()
{
    CPed* ped = m_Ped;
    CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
    auto weaponType = weapon.m_eWeaponType;

    if (weaponType != m_WeaponType)
    {
        m_WeaponType = weaponType;
        OnChangeWeapon();
    }

    if (!m_HasFiredGun && GetIsShooting())
    {
        m_HasFiredGun = true;
        OnFireGun();
    }

    if (!GetIsShooting())
    {
        m_HasFiredGun = false;
    }

    UpdateMuzzleFlash();
}

void Ped::UpdateMuzzleFlash()
{
    if (!Mod::m_EnableMuzzleFlashEffect) return;

    CPed* ped = m_Ped;
    int lightId = reinterpret_cast<unsigned int>(ped) + 500;

    if (m_CoronaTime > CTimer::m_snTimeInMilliseconds)
    {
        float radius = 1.0f;
        unsigned char alpha = 80;

        if (auto playerPed = FindPlayerPed(0))
        {
            if (DistanceBetweenPoints(ped->GetPosition(), playerPed->GetPosition()) > 10.0f)
            {
                //radius = 2.0f;
                alpha = 200;
            }
        }


        CVector position = GetSoundPosition();
        CCoronas::RegisterCorona(
            lightId,
            NULL,
            255,
            255,
            0,
            alpha,
            position,
            radius,
            800.0f,
            eCoronaType::CORONATYPE_SHINYSTAR,
            eCoronaFlareType::FLARETYPE_NONE,
            false,
            false,
            0,
            0.0f,
            false,
            0.01f,
            0,
            100.0f,
            false,
            false
        );

        /*
        int shadowId = (uintptr_t)ped + 100;

        float height = 1.0f;
        float width = 1.0f;

        CVector2D forward = CVector2D(ped->m_matrix->up);
        CVector2D_Normalize(&forward);

        CShadows::StoreShadowToBeRendered(
            (unsigned char)0,
            &ped->GetPosition(),
            height * forward.x,
            height * forward.y,
            width * forward.y,
            -width * forward.x,
            (short)255,
            (unsigned char)255,
            (unsigned char)255,
            (unsigned char)0
        );
        */
    }

    if (GetIsShooting())
    {
        CVector position = GetSoundPosition();
        Command< 0x09E5 >(position.x, position.y, position.z, 255, 255, 0, 10.0f);
    }
}

void Ped::Draw()
{
    CPed* ped = m_Ped;

    //if(GetIsShooting()) DrawWorldText("ped shooting", ped->GetPosition());

    //DrawWorldText("audio", GetSoundPosition());
}

void Ped::Destroy()
{
}

void Ped::OnFireGun()
{
    CPed* ped = m_Ped;
    CWeapon weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot];
    CWeaponInfo* weaponInfo = CWeaponInfo::GetWeaponInfo(weapon.m_eWeaponType, ped->m_nWeaponSkill);

    m_CoronaTime = CTimer::m_snTimeInMilliseconds + 100;

    if (Mod::m_EnableTracers)
    {
        TestTracers::AddTracer(
            TransformFromGunMuzzlePosition(m_Ped, CVector(0, 0, 0)),
            TransformFromGunMuzzlePosition(m_Ped, CVector(800, 0, 0)),
            m_TracerColor
        );
    }

    if (ped == FindPlayerPed(0))
    {
        auto dir = TheCamera.m_aCams[0].m_vecFront;
        dir.Normalise();

        //auto dir = TheCamera.m_aCams[0].m_vecFront;
        //dir.Normalise();

        //m_Ped->TransformFromObjectSpace(dir * 20.0f),
    }
    else {
    }

    

    

    float distanceMultiplier = 1.0f;

    //PrintMessage("pow", 2000);

    if (Mod::m_SoundSources.find(m_WeaponType) == Mod::m_SoundSources.end()) return;
    
    auto source = Mod::m_SoundSources[m_WeaponType];

    if (!source) return;

    vec3df pos3d(ped->GetPosition().x * distanceMultiplier, ped->GetPosition().z * distanceMultiplier, ped->GetPosition().y * distanceMultiplier);

    Mod::m_SoundEngine->play3D(source, pos3d);
}

void Ped::OnChangeWeapon()
{


    //PrintMessage("OnChangeWeapon", 2000);
}

bool Ped::GetIsShooting()
{
    CPed* ped = m_Ped;
    return ped->m_nPedFlags.bFiringWeapon;
}

CVector Ped::GetSoundPosition()
{
    return TransformFromGunMuzzlePosition(m_Ped, CVector(0, 0, 0));

    /*
    1
    
    CPed* ped = m_Ped;
    CVector audioSourcePos = ped->GetPosition();

    if (ped->m_pWeaponObject > 0)
    {
        auto objectMatrix = CMatrix(new RwMatrix(((RwFrame*)ped->m_pWeaponObject->parent)->modelling), true);
        audioSourcePos = objectMatrix.pos;
    }

    return audioSourcePos;
    */
}