#include "engine_pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "file/fastLoadScreen.h"
#include "systems/systems/windowManager.h"
#include "rendering/renderAPI.h"
#include "systems/systems/log.h"

namespace OwlEngine
{
	//! fls()
	void FastLoadScreen::fls()
	{
		//Leave unless we're in OpenGL
		if (RenderAPI::getAPI() != GraphicsAPI::OpenGL)
		{
			ENGINE_ERROR("Wrong API for loadscreen!");
			return;
		}

		//Setup Params: 
		unsigned int textureID, VAO, VBO, ID, vertex, fragment;
		int width, height, nrComponents;

		//LOAD IN TEXTURE WITH STBI
		glGenTextures(1, &textureID);

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("../sandbox/assets/textures/Owly_Fans_Window.png", &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(data);
		}
		else
		{
			ENGINE_ERROR("Cannot load Texture for loadscreen!");
			stbi_image_free(data);

		}

		float vertices[] =
		{
			-1.0, -1.0, 1.0,    0.0f, 0.0f,
			 1.0,  1.0, 1.0,    1.0f, 1.0f,
			-1.0,  1.0, 1.0,    0.0f, 1.0f,
			-1.0, -1.0, 1.0,    0.0f, 0.0f,
			 1.0, -1.0, 1.0,    1.0f, 0.0f,
			 1.0,  1.0, 1.0,    1.0f, 1.0f,
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(vertices)), vertices, GL_STATIC_DRAW);

		//xyz
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//UV
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//Create Shader
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			vShaderFile.open("../sandbox/assets/shaders/fastLoadScreen/fls.vert");
			fShaderFile.open("../sandbox/assets/shaders/fastLoadScreen/fls.frag");
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();


		}
		catch (std::ifstream::failure e)
		{
			ENGINE_ERROR("Cannot load Shaders for loadscreen!");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glUseProgram(ID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 30);

		WindowManager::getFocusedWindow()->getGraphicsContext()->swapBuffers();
		WindowManager::getFocusedWindow()->setDecorated(false);

		glDeleteTextures(1, &textureID);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(ID);
		glUseProgram(0);
	}
}
