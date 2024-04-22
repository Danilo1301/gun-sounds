#pragma once

#include "plugin.h"
#include "CBulletInfo.h"
#include "CMessages.h"
#include "CWorld.h"
#include "CWeapon.h"
#include "ePedBones.h"
#include "CSprite.h"
#include "CFont.h"
#include "extensions/ScriptCommands.h"

#include "map"
#include <fstream>

#pragma comment(lib, "irrKlang.lib")
#include "irrKlang.h"
using namespace irrklang;

#include "Log.h"

using namespace plugin;

static CVector CVector_Lerp(CVector A, CVector B, float t) {
	return A * t + B * (1.f - t);
}

static float CVector2D_MagnitudeSqr(CVector2D* vec2d) {
	return vec2d->x * vec2d->x + vec2d->y * vec2d->y;
}

static void CVector2D_Normalize(CVector2D* vec2d) {
	float sq = CVector2D_MagnitudeSqr(vec2d);
	if (sq > 0.0f) {
		float invsqrt = 1.0f / sqrt(sq);
		vec2d->x *= invsqrt;
		vec2d->y *= invsqrt;
	}
	else
		vec2d->x = 0.0f;
}

static void PrintMessage(std::string text, unsigned int time) {
	static char buffer[256];
	snprintf(buffer, sizeof(buffer), text.c_str());
	CMessages::AddMessageJumpQ(buffer, time, 0, false);
}

static void DrawWorldText(std::string text, CVector position, CRGBA color = CRGBA(255, 255, 0, 255)) {
	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetColor(color);
	CFont::SetDropShadowPosition(1);
	CFont::SetBackground(false, false);
	CFont::SetWrapx(500.0);
	CFont::SetScale(0.5 * 0.7f, 1.0 * 0.7f);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetProportional(true);

	RwV3d rwp = { position.x, position.y, position.z };
	RwV3d screenCoors; float w, h;

	CSprite::CalcScreenCoors(rwp, &screenCoors, &w, &h, true, true);

	char buffer[1024];
	sprintf(buffer, "%s", text.c_str());
	CFont::PrintString(screenCoors.x, screenCoors.y, buffer);
}

static void DrawScreenText(std::string text, CVector2D position, CRGBA color = CRGBA(255, 255, 0, 255)) {
	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetColor(color);
	CFont::SetDropShadowPosition(1);
	CFont::SetBackground(false, false);
	CFont::SetWrapx(500.0);
	CFont::SetScale(0.5 * 0.7f, 1.0 * 0.7f);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetProportional(true);

	char buffer[1024];
	sprintf(buffer, "%s", text.c_str());
	CFont::PrintString(position.x, position.y, buffer);
}

static std::string to_lower(std::string data) {
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::tolower(c);
	});
	return data;
}

inline bool file_exists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}