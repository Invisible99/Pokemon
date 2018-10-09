#pragma once
#include "Level.h"
#include "Pokemon.h"
class Character
{
public:
	Character();
	
	void Update(float elapsedSec, const Level& level, Battle& battle);
	void Draw() const;

	Rectf& GetShape();
	std::vector<std::shared_ptr<TeamPokemon>>& GetPokemons();
	bool& IsTalking();
	Vector2f& GetVelocity();

	void CreateRandomTeam();

	enum ActionState {
		waiting,
		moving
	};

	enum MovingWay {
		Left,
		Right,
		Up,
		Down
	};

private:
	//AnimationVariables
	Texture m_SpritesTexture;
	const float m_ClipHeight = 20.0f;
	const float m_ClipWidth = 16.0f;
	const int m_NrOfFrames = 3;
	const int m_NrFramesPerSec = 6;
	float m_AnimTime;
	int m_AnimFrame;
	const float m_Speed;

	Point2f m_StartPos;

	Rectf m_Shape;

	bool m_LedgeFound;
	float m_LedgeAnimTime;
	float m_DistancesLedge;
	float m_ToLedge;
	float m_LedgeSpeed{ 3 };

	Vector2f m_Velocity;

	ActionState m_ActionState;
	bool m_MovingRight;
	
	bool m_IsTalking{ false };
	std::vector<std::shared_ptr<TeamPokemon>> m_pPokemons;
	
	//Functions
	void ButtonPresses(const Uint8 *pKeysStates);
	void Animate(float elapsedSec, int frames);
	void AnimateLedge(float elapsedSec);
};