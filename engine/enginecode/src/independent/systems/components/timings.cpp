/*! \file timings.cpp
*
* \brief Stores a lot of timing data to be displayed in a graph
*
* \author Joshua Simons
*
*/
#include "engine_pch.h"
#include "systems/components/timings.h"

namespace OwlEngine
{
	//! allocate
	void Timings::allocate()
	{
		if (m_start != nullptr) destroy();

		int size = m_storeSize + m_displaySize + 1;
		m_start = (float*)malloc(size * sizeof(float));

		for (int i = 0; i < size; i++)
		{
			m_start[i] = 0.0f;
		}
	}
	
	//! destroy()
	void Timings::destroy()
	{
		if (m_start != nullptr) free(m_start);
	}

	//! Timings()
	Timings::Timings()
	{
		allocate();
	}

	//! ~Timings()
	Timings::~Timings()
	{
		destroy();
	}

	//! setValue
	/*!
	\param val a float - The time value
	*/
	void Timings::setValue(float val)
	{
		m_index++;
		if (m_index >= m_storeSize) m_index -= (m_storeSize);

		uint32_t location = m_index + m_displaySize;

		m_start[location] = val;
		if (location >= m_storeSize)
		{
			location -= (m_storeSize);
			m_start[location] = val;
		}
	}

	//! getValues()
	/*!
	\return a float* - A pointer to the floats
	*/
	float* Timings::getValues()
	{
		return &m_start[m_index];
	}

	//! getIndex()
	/*!
	\return an uint32_t - The index
	*/
	uint32_t Timings::getIndex()
	{
		return m_index;
	}

	//! getCount()
	/*!
	\return an uint32_t - The count
	*/
	uint32_t Timings::getCount()
	{
		return m_displaySize;
	}
}