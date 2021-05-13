/* \file monster.h
*
*	\brief The monster script
*
*	\author: Jamie Hall
*
*/
#ifndef MONSTER_H
#define MONSTER_H

#include "independent/entities/components/nativeScript.h"
#include "scripts/menus/gameOverScript.h"

using namespace OwlEngine;

namespace PatrolType {
	/*! \enum PatrolType
		* \brief The different methods of patrol
		*/
	enum PatrolNavigateType
	{
		CIRCUIT,	// Circuit patrol - return to node 1 after finish
		TRACK		// Track patrol - invert traversal order after finish
	};
}
/*!	\class Monster
*	\brief The monster script
*/
class Monster : public NativeScript
{
private:
	PatrolType::PatrolNavigateType m_patrolType;	//!< The patrol type used by this monster
	std::vector<Entity*> m_movementNodes;	//!< Vector storing the movement node entities
	std::vector<Entity*> m_globalEntities;	//!< Vector storing all entities in scene
	Entity* m_gameOverEnt = nullptr;	//!< Game over entity
	Entity* m_playerEnt = nullptr;	//!< Player entity
	GameOverScript* m_gameOverScript;	//!< Pointer to game over object
	uint32_t m_iterator;	//!< Iterator used to traverse movement node array
	float m_velocity;		//!< Monsters velocity
	float m_detectDist; //!< how far the moster will look for you
	bool m_reachedTarget = false;	//!< Has the monster reached its target
	bool m_invertTraversal = false;	//!< Used to flag returning traversal for TRACK patrol type
	bool m_huntingEnabled = false;	//!< Flag for player seeking behaviour	
	bool m_killingEnabled = false;	//!< Flag to check if monster can kill player


public:
	Monster();	//!< Constructor
	Monster(nlohmann::json scriptData); //!< Constructor
	~Monster();	//!< Destructor

	nlohmann::ordered_json getAsJSON() override; //!< Get the script in JSON format
	void onAttach() override;	//!<
	void onPreUpdate(float timestep, float totalTime) override;		//!< Call before game update
	void onPostUpdate(float timestep, float totalTime) override;	//!< Call after game update
	void onTrigger(Entity* overlapEntity) override;		//!< On overlap with RP3D trigger

#ifndef NG_DEPLOY
	void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY

	void moveToNode(Entity* targetNode, float timestep);	//!< Move towards a given target entity
	void updateMovementNodeVector();	//!< Call to update the monsters attached movement nodes
};
#endif