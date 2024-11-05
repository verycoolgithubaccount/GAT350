#pragma once
#include <chrono>

class ETime
{
private:
	using clock = std::chrono::high_resolution_clock;

	float m_time{ 0 };
	float m_deltaTime{ 0 };

	clock::time_point m_startTime;
	clock::time_point m_frameTime;

public:
	ETime() :
		m_startTime{ clock::now() },
		m_frameTime{ clock::now() }
	{}

	void Tick();
	void Reset() { m_startTime = clock::now(); }

	float GetTime() const { return m_time; }
	float GetDeltaTime() const { return m_deltaTime; }
	float GetElapsedTime() const;
};