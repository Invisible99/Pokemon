#include "stdafx.h"
#include "Character.h"
#include "PokemonManager.h"

Character::Character()
	: m_Shape{ Rectf(50,130, m_ClipWidth, m_ClipHeight) }
	, m_Speed{ 32.0f }
	, m_Velocity{ Vector2f(0.0f,0.0f) }
	, m_ActionState{ moving }
	, m_SpritesTexture{ "Resources/Images/CharBoy.png"}
	, m_StartPos{ Point2f(0, 33) }
	, m_AnimTime {0}
	, m_AnimFrame{ 0 }
{
	
}

void Character::Update(float elapsedSec, const Level& level, Battle& battle) {
	const Uint8 *pKeysStates = SDL_GetKeyboardState(nullptr);
	switch (m_ActionState) {
	case waiting:
		ButtonPresses(pKeysStates);
		if (!pKeysStates[SDL_SCANCODE_W] && !pKeysStates[SDL_SCANCODE_S] && !pKeysStates[SDL_SCANCODE_A] && !pKeysStates[SDL_SCANCODE_D]) {
			m_Velocity.x = 0;
			m_Velocity.y = 0;
		}
		break;
	case moving:
		ButtonPresses(pKeysStates);
		if (!pKeysStates[SDL_SCANCODE_W] && !pKeysStates[SDL_SCANCODE_S] && !pKeysStates[SDL_SCANCODE_A] && !pKeysStates[SDL_SCANCODE_D]) {
			m_ActionState = waiting;
		}
		Animate(elapsedSec, m_NrOfFrames);
		break;
	}

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	level.HandleCollision(m_Shape, m_Velocity, m_LedgeFound, m_DistancesLedge, m_ToLedge);
	level.CheckGrass(m_Shape, m_Velocity, m_ActionState, battle, m_pPokemons);

	if (m_LedgeFound) {
		AnimateLedge(elapsedSec);
	}
}

void Character::Draw() const{
	switch (m_ActionState) {
	case waiting:
		glPushMatrix();
		if (m_MovingRight) {
	    	glTranslatef(m_Shape.left + m_ClipWidth / 2.0f, 0.0f, 0.0f);
			glScalef(-1.0f, 1.0f, 1.0f);
			glTranslatef(-m_Shape.left - m_ClipWidth / 2, 0.0f, 0.0f);		
		}
		m_SpritesTexture.Draw(m_Shape, Rectf{ m_StartPos.x + m_ClipWidth, m_StartPos.y, m_ClipWidth,m_ClipHeight });
	
		glPopMatrix();
		break;
	case moving:
		glPushMatrix();
		if (m_MovingRight) {
			glTranslatef(m_Shape.left + m_ClipWidth / 2.0f, 0.0f, 0.0f);
			glScalef(-1.0f, 1.0f, 1.0f);
			glTranslatef(-m_Shape.left - m_ClipWidth / 2, 0.0f, 0.0f);
		}
		m_SpritesTexture.Draw(m_Shape, Rectf{ m_StartPos.x + (m_ClipWidth * m_AnimFrame) , m_StartPos.y, m_ClipWidth, m_ClipHeight });
	
		glPopMatrix();
		break;
	}
}

Rectf& Character::GetShape() {
	return m_Shape;
}

void Character::ButtonPresses(const Uint8 *pKeysStates) {
	m_Velocity.x = 0;
	m_Velocity.y = 0;
	if (!m_LedgeFound) {
		if (pKeysStates[SDL_SCANCODE_W]) {
			m_ActionState = moving;
			m_MovingRight = false;
			m_StartPos.y = 65;
			m_Velocity.y = m_Speed;
		}
		else if (pKeysStates[SDL_SCANCODE_S]) {
			m_ActionState = moving;
			m_MovingRight = false;
			m_StartPos.y = 33;
			m_Velocity.y = -m_Speed;
		}
		else if (pKeysStates[SDL_SCANCODE_D]) {
			m_ActionState = moving;
			m_MovingRight = true;
			m_StartPos.y = 96;
			m_Velocity.x = m_Speed;
		}
		else if (pKeysStates[SDL_SCANCODE_A]) {
			m_ActionState = moving;
			m_MovingRight = false;
			m_StartPos.y = 96;
			m_Velocity.x = -m_Speed;
		}
	}
}

void Character::Animate(float elapsedsec, int frames) {
	m_AnimTime += elapsedsec;
	if (m_AnimTime >= (1.0f / m_NrFramesPerSec)) {
		if (m_AnimFrame != frames) {
			++m_AnimFrame;
		}
		else {
			m_AnimFrame = 0;
		}
		m_AnimTime = 0;
	}
}

void Character::AnimateLedge(float elapsedSec)
{
	m_Shape.bottom -= m_DistancesLedge * m_LedgeSpeed * elapsedSec;
	if (m_Shape.bottom <= m_ToLedge)
	{
		m_LedgeFound = false;
	}
}

void Character::CreateRandomTeam() {
	m_pPokemons.push_back( std::make_shared<TeamPokemon>(PokemonManager::GetInstance().GetPokemon(0), 5)); 
	m_pPokemons.push_back(std::make_shared<TeamPokemon>(PokemonManager::GetInstance().GetPokemon(rand() % 146), 5)); 
	m_pPokemons.push_back(std::make_shared<TeamPokemon>(PokemonManager::GetInstance().GetPokemon(rand() % 146), 5));
}

std::vector<std::shared_ptr<TeamPokemon>>& Character::GetPokemons()
{
	return m_pPokemons;
}

bool & Character::IsTalking()
{
	return m_IsTalking;
}

Vector2f & Character::GetVelocity()
{
	return m_Velocity;
}
