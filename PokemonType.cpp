#include "stdafx.h"
#include "PokemonType.h"

PokemonType::PokemonType()
{
	m_pTypeTexts.push_back(std::make_shared<std::string>("None"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Normal"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Fire"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Fighting"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Flying"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Grass"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Poison"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Electric"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Ground"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Psychic"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Rock"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Ice"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Bug"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Dragon"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Ghost"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Dark"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Steel"));
	m_pTypeTexts.push_back(std::make_shared<std::string>("Fairy"));
}

PokemonType& PokemonType::GetInstance() {
	static PokemonType* pSingleton = new PokemonType();
	return *pSingleton;
}

std::shared_ptr<std::string> PokemonType::GetTypeText(int id)
{
	return m_pTypeTexts[id];
}
