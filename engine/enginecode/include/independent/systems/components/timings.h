/*! \file timings.h
*
* \brief Stores a lot of timing data to be dispalyed in a graph
*
* \author Joshua Simons
*
*/
#ifndef TIMINGS_H
#define TIMINGS_H

namespace OwlEngine
{
	/*! \class Timings
	* \brief Stores a lot of timing data to be displayed in a graph
	*/
	class Timings
	{
	private:
		float* m_start = nullptr; //!< Start
		uint32_t m_storeSize = 256; //!< The store size
		uint32_t m_displaySize = 128; //!< The display size
		uint32_t m_index = 0; //!< The current index

		void allocate(); //!< Allocate
		void destroy(); //!< Destroy
	public:
		Timings(); //!< Constructor
		~Timings(); //!< Destructor

		void setValue(float val); //!< Set the value of a time
		float* getValues(); //!< Get the value of a time

		uint32_t getIndex(); //!< Get the index of a time
		uint32_t getCount(); //!< Get the count
	};
}
#endif