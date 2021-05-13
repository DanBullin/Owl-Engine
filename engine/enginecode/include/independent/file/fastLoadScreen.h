/*! \file fastLoadScreen.h
*
* \brief Fast Load Screen for the purpose of loading one image to the screen only whilst everything else loads in
*
* \author Joshua Simons
*
*/
#ifndef FASTLOADSCREEN_H
#define FASTLOADSCREEN_H

namespace OwlEngine
{
	/*! \class FastLoadScreen
	* \brief Displays load screen
	*/
	class FastLoadScreen
	{
	public:
		static void fls(); //!< Displays load screen to screen fastly
	};
}

#endif