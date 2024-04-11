#include "TestTracers.h"

#include "CCoronas.h"

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
	int lightId = 2514854;

	float f = 2.0f;

	int removeTracer = -1;

	int i = -1;
	for (auto &tracer : m_Tracers)
	{
		i++;

		tracer.pos += 0.001f * f;
		if (tracer.pos > 1) {
			removeTracer = i;
			continue;
		}

		/*
		float norm = 30.0f / info->m_fRange;
		CVector traceTarget = (target - source) * norm + source;
		*/

		//RegisterCorona(lightId++, tracer.start, CRGBA(255, 0, 0), 0.1f);

		for (int i = 0; i < 10; i++)
		{
			auto newPos = tracer.pos + (i * 0.0003f / f);

			RegisterCorona(lightId++, CVector_Lerp(tracer.end, tracer.start, newPos), tracer.color, 0.15f);
		}

		//RegisterCorona(lightId++, tracer.end, CRGBA(0, 255, 0), 0.1f);
	}

	if(removeTracer != -1)
	{
		RemoveTracer(removeTracer);
	}
}