#pragma once
class PokemonType
{
public:
	enum Types {
		None,
		Normal,
		Fire,
		Fighting,
		Water,
		Flying,
		Grass,
		Poison,
		Electric,
		Ground,
		Psychic,
		Rock,
		Ice,
		Bug,
		Dragon,
		Ghost,
		Dark,
		Steel,
		Fairy
	};
	static PokemonType & GetInstance();
	std::shared_ptr<std::string> GetTypeText(int id);
private:
	PokemonType();
	std::vector<std::shared_ptr<std::string>> m_pTypeTexts;
};

