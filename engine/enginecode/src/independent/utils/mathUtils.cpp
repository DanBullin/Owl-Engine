/*! \file mathUtils.cpp
*
* \brief A math utility class to help with data regarding maths
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/utils/mathUtils.h"

namespace OwlEngine
{
	//! convertMat4ToArray()
	/*!
	\param data a const glm::mat4& - The data to convert
	\return a std::array<double, 16> - The matrix data in array format
	*/
	std::array<double, 16> MathUtils::convertMat4ToArray(const glm::mat4& data)
	{
		std::array<double, 16> matArray;

		const float *pSource = (const float*)glm::value_ptr(data);
		for (int i = 0; i < 16; ++i)
			matArray[i] = pSource[i];
		return matArray;
	}

	//! getModelMatrix()
	/*!
	\param pos a const glm::vec3& - The position
	\return a mat4 - The model matrix
	*/
	glm::mat4 MathUtils::getModelMatrix(const glm::vec3& pos)
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, pos);
		model = glm::scale(model, {1.f, 1.f, 1.f});
		return model;
	}

	//! getModelMatrix()
	/*!
	\param pos a const glm::vec3& - The position
	\param scale a const glm::vec3& - The scale
	\return a mat4 - The model matrix
	*/
	glm::mat4 MathUtils::getModelMatrix(const glm::vec3 & pos, const glm::vec3& scale)
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, pos);
		model = glm::scale(model, scale);
		return model;
	}

	//! getModelMatrix()
	/*!
	\param pos a const glm::vec3& - The position
	\param rotation a const glm::vec3& - The rotation
	\param scale a const glm::vec3& - The scale
	\return a mat4 - The model matrix
	*/
	glm::mat4 MathUtils::getModelMatrix(const glm::vec3 & pos, const glm::vec3 & rotation, const glm::vec3 & scale)
	{
		// Order: Translate then Rotation then Scale
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, pos);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, scale);
		return model;
	}
}