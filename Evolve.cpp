#include "stdafx.h"
#include "Evolve.h"
#include "PokemonManager.h"
#include "AudioManager.h"

Evolve::Evolve()
	: m_Spritesheet { "Resources/Images/Evolving.png" }
	, m_ColorWhite {Color4f(1.0f,1.0f,1.0f,1.0f)}
{
}

bool Evolve::GetEvolving()
{
	return m_Evolving;
}

void Evolve::StartEvolving(std::shared_ptr<Pokemon> toEvolve)
{
	m_ToEvolve = toEvolve;

	m_Evolving = true;
	m_Evolved = false;
	m_White = false;

	m_TimeEvolving = 0.0f;
	m_TimeToEvolve = 5.0f;

	m_TimeLastSwitch = 0.0f;
	m_TimeToSwitch = 0.5f;

	m_TimeEvolved = 0.0f;
	m_TimeToEvolved = 2.0f;

	AudioManager::GetInstance().PlayEvolving();
}

Evolve& Evolve::GetInstance() {
	static Evolve* pSingleton = new Evolve();
	return *pSingleton;
}

void Evolve::Update(float elapsedSec)
{
	m_TimeEvolving += elapsedSec;
	m_TimeLastSwitch += elapsedSec;
	if (m_TimeLastSwitch >= m_TimeToSwitch && !m_Evolved) {
		if (m_White) {
			m_ColorWhite = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
			m_White = false;
		}
		else {
			m_ColorWhite = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
			m_White = true;
		}
		m_TimeToSwitch -= 0.025f;
		m_TimeLastSwitch = 0.0f;
	}
	if (m_TimeEvolving >= m_TimeToEvolve && !m_Evolved) {
		//m_TimeEvolving = 0.0f;
		m_Evolved = true;
		m_ColorWhite = Color4f(0.0f, 0.0f, 0.0f, 1.0f);
		m_ToEvolve = PokemonManager::GetInstance().GetPokemon(m_ToEvolve->GetIndexProEvo());
	}

	if (m_Evolved) {
		m_TimeEvolved += elapsedSec;

		if (m_TimeEvolved >= m_TimeToEvolved) {
			m_Evolving = false;
			AudioManager::GetInstance().PlayOverworld();
		}
	}
}

void Evolve::Draw() const
{
	m_Spritesheet.Draw(Rectf(0, 0, m_Width, m_Height), Rectf(0, 0, m_Width, m_Height));
	
	PokemonManager::GetInstance().GetSpriteSheet()->Draw(
		Rectf(m_PokPos.x, m_PokPos.y, PokemonManager::GetInstance().GetWidth(), PokemonManager::GetInstance().GetHeight()),
		Rectf(m_ToEvolve->GetStartPos().x, m_ToEvolve->GetStartPos().y, PokemonManager::GetInstance().GetWidth(), PokemonManager::GetInstance().GetHeight()),
		m_ColorWhite);
}
