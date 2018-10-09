#pragma once
#include "TeamPokemon.h"
class Evolve
{
public:
	static Evolve& GetInstance();

	void Update(float elapsedSec);
	void Draw() const;
	
	bool GetEvolving();
	void StartEvolving(std::shared_ptr<Pokemon> toEvolve);
private:
	explicit Evolve();
	
	Texture m_Spritesheet;
	std::shared_ptr<Pokemon> m_ToEvolve;
	float m_Width{ 240.0f };
	float m_Height{ 160.0f };
	Point2f m_PokPos{ Point2f(88.0f,70.0f) };
	Color4f m_ColorWhite;

	bool m_Evolving{ false };
	bool m_Evolved{ false };
	bool m_White{ false };

	float m_TimeEvolving{ 0.0f };
	float m_TimeToEvolve{ 5.0f };

	float m_TimeLastSwitch{ 0.0f };
	float m_TimeToSwitch{ 0.5f };

	float m_TimeEvolved{ 0.0f };
	float m_TimeToEvolved{ 2.0f };
};

