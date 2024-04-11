#include "Peds.h"

std::map<CPed*, Ped*> Peds::m_Peds;

bool Peds::PedExists(CPed* p) {
	return m_Peds.find(p) != m_Peds.end();
}

void Peds::CreatePed(CPed* p) {
	Ped* ped = new Ped(p);
	m_Peds.insert(std::pair<CPed*, Ped*>(p, ped));
}

Ped* Peds::GetPed(CPed* p) {
	return m_Peds.at(p);
}

void Peds::RemovePed(CPed* p) {
	Ped* ped = GetPed(p);
	ped->Destroy();
	m_Peds.erase(p);
}