#include "TestTracers.h"

#include "CCoronas.h"

#include "Mod.h"

std::vector<Tracer> TestTracers::m_Tracers;

void RegisterCorona(int lightid, CVector position, CRGBA color, float radius) {
	CCoronas::RegisterCorona(
		lightid,
		NULL,
		color.r,
		color.g,
		color.b,
		color.a,
		position,
		radius,
		500.0f,
		eCoronaType::CORONATYPE_SHINYSTAR,
		eCoronaFlareType::FLARETYPE_NONE,
		false,
		false,
		0,
		0.0f,
		false,
		0.1f,
		0,
		100.0f,
		false,
		false
	);
}

void TestTracers::AddTracer(CVector start, CVector end, CRGBA color)
{
	Tracer tracer;
	tracer.color = color;
	tracer.start = start;
	tracer.end = end;
	tracer.pos = 0;

	m_Tracers.push_back(tracer);
}

void TestTracers::RemoveTracer(int index)
{
	m_Tracers.erase(m_Tracers.begin() + index);
}

void TestTracers::Update()
{
	int lightId = 25148;

	float f = 2.0f; //maybe somethign about velocity? i dont remember

	int removeTracer = -1;

	int i = -1;
	for (auto &tracer : m_Tracers)
	{
		i++;

		tracer.pos += 0.001f * f;
		if (tracer.pos >= 1) {
			removeTracer = i;
			continue;
		}

		/*
		float norm = 30.0f / info->m_fRange;
		CVector traceTarget = (target - source) * norm + source;
		*/

		//RegisterCorona(lightId++, tracer.start, CRGBA(255, 0, 0), 0.1f);

		int length = (int)round(10 * Mod::m_TracerLength);

		for (int i = 0; i < length; i++)
		{
			auto newPos = tracer.pos + (i * Mod::m_TracerCoronaDistanceSpace / f);
			
			RegisterCorona(lightId++, CVector_Lerp(tracer.end, tracer.start, newPos), tracer.color, 0.15f * Mod::m_TracerSize);
		}

		//RegisterCorona(lightId++, tracer.end, CRGBA(0, 255, 0), 0.1f);
	}

	if(removeTracer != -1)
	{
		RemoveTracer(removeTracer);
	}
}