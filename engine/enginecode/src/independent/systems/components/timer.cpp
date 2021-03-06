/*! \file timer.cpp
*
* \brief A class to measure time
*
* \author DMU Course material
*
*/
#include "engine_pch.h"
#include "independent/systems/components/timer.h"
#include "independent/systems/systems/log.h"

namespace OwlEngine
{
	//! ~Timer()
	Timer::~Timer()
	{

	}

	//! ChronoTimer()
	/*!
	\param timerName a const char* - The name of the timer
	*/
	ChronoTimer::ChronoTimer(const char* timerName) : m_timerName(timerName)
	{
		ENGINE_TRACE("[ChronoTimer::ChronoTimer] Creating a timer named: {0}.", m_timerName);
	}

	//! ~ChronoTimer()
	ChronoTimer::~ChronoTimer()
	{
		ENGINE_TRACE("[ChronoTimer::~ChronoTimer] Destroying a timer named: {0}.", m_timerName);
	}

	//! getElapsedTime()
	/*!
	\return a const float - The time in seconds
	*/
	const float ChronoTimer::getElapsedTime()
	{
		// Measure the time based on the end point and the start
		m_endPoint = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> elapsed = m_endPoint - m_startPoint;
		// Return in seconds
		return elapsed.count() / 1000.f;
	}
}