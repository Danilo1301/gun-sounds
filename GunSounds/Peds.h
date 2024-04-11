#pragma once

#include "pch.h"
#include "Ped.h"

class Peds {
public:
	static std::map<CPed*, Ped*> m_Peds;

	static bool PedExists(CPed* ped);
	static void CreatePed(CPed* ped);
	static Ped* GetPed(CPed* ped);
	static void RemovePed(CPed* ped);
};