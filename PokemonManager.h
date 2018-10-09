#pragma once
#include "Pokemon.h"
class PokemonManager
{
public:	
	static PokemonManager & GetInstance();
	void LoadPokemons();

	std::shared_ptr<Texture>& GetSpriteSheet();
	float GetWidth();
	float GetHeight();
	std::shared_ptr<Pokemon> GetPokemon(int id);

private:
	PokemonManager();
	std::string m_Filename;
	std::shared_ptr<Texture> m_pPokemonsprite; 
	const float m_Width;
	const float m_Height;
	std::vector<std::shared_ptr<Pokemon>> m_pPokemons;
};

