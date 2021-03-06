/*! \file renderer3D.h
*
* \brief A 3D renderer which allows 3D models to be submitted and drawn (Batched)
*
* \author Daniel Bullin
*
*/
#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "independent/core/common.h"
#include "independent/rendering/geometry/vertex.h"
#include "independent/rendering/geometry/vertexArray.h"
#include "independent/rendering/geometry/indirectBuffer.h"
#include "independent/rendering/uniformBuffer.h"
#include "independent/rendering/textures/textureUnitManager.h"
#include "independent/rendering/renderUtils.h"
#include "independent/rendering/geometry/mesh3D.h"
#include "independent/rendering/materials/material.h"
#include "independent/entities/components/meshRender3D.h"

namespace OwlEngine
{
	/*! \struct BatchEntry3D
	* \brief A struct containing the 3D submission data
	*/
	struct BatchEntry3D
	{
		std::string submissionName; //!< The submission name
		Geometry3D geometry; //!< The geometry
		ShaderProgram* shader; //!< The shader program
		std::vector<SubTexture*> subTextures; //!< The list of subtextures in this entry
		std::vector<CubeMapTexture*> cubeTextures; //!< The list of cubemap textures in this entry
		std::vector<int32_t> textureUnits; //!< The list of texture units used subtextures
		std::vector<int32_t> cubeTextureUnits; //!< The list of texture units used for cubemaps
		float shininess; //!< The shininess of the material
		glm::mat4 modelMatrix; //!< The model matrix
		glm::vec4 tint; //!< The tint
	};

	/*! \class Renderer3D
	* \brief A static class which renders 3D geometry (Batched)
	*/
	class Renderer3D
	{
	private:
		static TextureUnitManager* s_unitManager; //!< The texture unit manager
		static std::array<int32_t, 16> s_unit; //!< The texture units
		static uint32_t s_batchCapacity; //!< The limit of submissions in a batch before we need to flush
		static uint32_t s_vertexCapacity; //!< The limit of the number of vertices in the vertex buffer
		static uint32_t s_indexCapacity; //!< The limit of the number indices in the index buffer

		static std::vector<BatchEntry3D> s_batchQueue; //!< The list of submitted geometry in a queue
		static std::map<VertexBuffer*, std::vector<DrawElementsIndirectCommand>> s_batchCommandsQueue; //!< The list of batch commands
		static std::map<VertexBuffer*, uint32_t> s_nextVertex; //!< The next vertex (index) in the vertex buffer where we can add new vertices
		static uint32_t s_nextIndex; //!< The next index (index) in the index buffer where we can add new indices
		static IndirectBuffer* s_indirectBuffer; //!< The indirect buffer containing all batch commands

		static bool submissionChecks(Material* material, Geometry3D& geom); //!< Check the submission
		static void sortSubmissions(std::vector<BatchEntry3D>& submissions); //!< Sort the submissions

		static bool drawCheck(ShaderProgram* program, std::unordered_map<std::string, UniformBuffer*>& buffers, VertexArray* vArray, IndirectBuffer* indirectBuffer);
		static void clearBatch(); //!< Clear current batch
		static void flushBatch(); //!< Flush the current batch queue
		static void flushBatchCommands(std::vector<BatchEntry3D>& queue); //!< Flush the current batch commands
	public:
		static void initialise(const uint32_t batchCapacity, const uint32_t vertexCapacity, const uint32_t indexCapacity); //!< Initialise the renderer
		static void begin(); //!< Begin a new 3D scene
		static void submit(const std::string& submissionName, Geometry3D geometry, Material* material, const glm::mat4& modelMatrix); //!< Submit a piece of geometry to render
		static void end(); //!< End the current 3D scene
		static void destroy(); //!< Destroy all internal data
		static void setTextureUnitManager(TextureUnitManager*& unitManager, const std::array<int32_t, 16>& unit); //!< Set the texture unit manager and units to use

		static void addGeometry(std::vector<Vertex3D>& vertices, std::vector<uint32_t> indices, Geometry3D& geometry); //!< Add a piece of 3D geometry to the renderer's vertex buffer
	};
}
#endif