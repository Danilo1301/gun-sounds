#pragma once

#include "pch.h"

class Ped {
private:
	bool m_HasFiredGun = false;
	eWeaponType m_WeaponType = eWeaponType::WEAPON_UNARMED;
	unsigned int m_CoronaTime = 0;
public:
	CPed* m_Ped;
	CRGBA m_TracerColor;

	Ped(CPed* ped);

	void Update();
	void UpdateMuzzleFlash();
	void Draw();
	void Destroy();

private:
	void OnFireGun();
	void OnChangeWeapon();
	void ProcessTracers();

public:
	bool GetIsShooting();
	CVector GetSoundPosition();
};