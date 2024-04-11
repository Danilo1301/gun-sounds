#pragma once

#include "pch.h"

struct Tracer {
	CRGBA color;
	CVector start;
	CVector end;
	float pos;
};

class TestTracers {
public:
	static std::vector<Tracer> m_Tracers;

	static void AddTracer(CVector start, CVector end, CRGBA color);
	static void RemoveTracer(int index);
	static void Update();
};