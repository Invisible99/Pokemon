#pragma once
#include "TeamPokemon.h"
#include "Battle.h"
class Enemie
{
public:
	enum Looking {
		Down,
		Up,
		Left,
		Right
	};
	explicit Enemie(Point2f startPos, Point2f shape, int length, int gemLevel);
	void Update(float elapsedSec, Rectf& charShape, bool& isTalking, Vector2f& actorVelocity);
	void Draw(Point2f& camPos) const;
	void DrawTextbox(Point2f& camPos) const;

	void CheckToStartBattle(Battle& battle, std::vector<std::shared_ptr<TeamPokemon>>& charTeam, bool& isTalking);
private:
	Texture m_SpritesTexture;
	Texture m_TextboxTexture;
	const float m_ClipHeight = 20.0f;
	const float m_ClipWidth = 15.0f;
	const float m_TextboxWidth = 244.0f;
	const float m_TextboxHeight = 44.0f;

	TTF_Font* m_pFontText;

	Rectf m_Shape;
	Point2f m_StartPos;
	
	std::vector<std::shared_ptr<TeamPokemon>> m_pPokemons;
	int m_GemLevel;
	float m_SecondsPast{ 0 };
	Looking m_LookingWay;

	bool m_HasBattled{ false };
	bool m_IsTalking{ false };

	void ChangeLookingDirection(float elapsedSec);
	void CreateRandomTeam(int length);
	void CheckForBattle(Rectf& charShape, bool& isTalking);
	void InitiatBattle(bool& isTalking);

	void CheckTrainer(Rectf& actorShape, Vector2f& actorVelocity);
};

