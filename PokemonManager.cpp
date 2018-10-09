#include "stdafx.h"
#include "PokemonManager.h"
#include "Split.h"
#include <fstream>
#include <iostream>


PokemonManager::PokemonManager(): m_Width{64.0f}, m_Height{64.0f}
{	
	m_Filename = "Resources/Text/Pokemon.csv";
	m_pPokemonsprite = std::make_shared<Texture>("Resources/Images/PokemonSprite.png");
}

PokemonManager& PokemonManager::GetInstance() {
	static PokemonManager* pSingleton = new PokemonManager();
	return *pSingleton;
}


void PokemonManager::LoadPokemons() {
	std::ifstream pokemonFile{ m_Filename };

	if (!pokemonFile) {
		std::cout << "Openen van bestand mislukt: " << m_Filename << "\n";
		std::cin.get();
		return;
	}

	std::string pokemonStr{};
	int id{ 0 };
	while (pokemonFile && std::getline(pokemonFile, pokemonStr))
	{
		std::vector<std::string> pokemonVec = explodeString(pokemonStr, ',');

		m_pPokemons.push_back(std::make_shared<Pokemon>(pokemonVec[0], std::stoi(pokemonVec[1]), std::stoi(pokemonVec[2]), std::stof(pokemonVec[3]), std::stof(pokemonVec[4]), std::stof(pokemonVec[5]), std::stof(pokemonVec[6]),
			std::stof(pokemonVec[7]), std::stof(pokemonVec[8]), std::stof(pokemonVec[9]), std::stoi(pokemonVec[10]), std::stoi(pokemonVec[11]),	std::stoi(pokemonVec[12]), std::stof(pokemonVec[13]), pokemonVec[14], 
			std::stof(pokemonVec[15]), std::stof(pokemonVec[16])));
		for (size_t i{ 17 }; i < pokemonVec.size() - 2; i += 2) {
			m_pPokemons[id]->AddMove(std::stoi(pokemonVec[i]), std::stoi(pokemonVec[i + 1]));
		}		
		++id;
	}
}

std::shared_ptr<Texture>& PokemonManager::GetSpriteSheet() {
	
	return m_pPokemonsprite;
}

float PokemonManager::GetWidth() {
	return m_Width;
}

float PokemonManager::GetHeight() {
	return m_Height;
}

std::shared_ptr<Pokemon> PokemonManager::GetPokemon(int id) {
	return m_pPokemons[id];
}