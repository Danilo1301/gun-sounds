#pragma once

#include "pch.h"

class ModConfig {
public:
	static std::string GetFullPath(std::string path);

	static void Load();
	static void LoadSettings();
};