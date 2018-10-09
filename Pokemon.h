#pragma once
#include "PokemonType.h"
#include "Move.h"
#include "StatusEffects.h"
#include <deque>
class Pokemon
{
public:
	explicit Pokemon(std::string name, int mainType, int subType, float hp, float att, float def, float spAttack, float spDef, float speed, float total, int preEvo, int proEvo, int level, float genderRatio, 
		std::string soundFile, float xPos, float yPos);
	void AddMove(int level, int moveID);

	std::string GetName();

	Point2f GetStartPos();

	float GetHP();
	float GetAttack();
	float GetSpAttack();
	float GetDefense();
	float GetSpDefense();
	float GetSpeed();
	int GetIndexPreEvo();
	int GetIndexProEvo();
	int GetLevelUp();

	const std::vector< std::shared_ptr<Move>>& GetMoveset();
	const std::vector<int>& GetLevelLearned();
private:
	//Texture
	Point2f m_StartPos;

	//Moveset
	std::vector< std::shared_ptr<Move>> m_pMoves;
	std::vector<int> m_LevelsLearned;

	//Type
	//SubType
	PokemonType::Types m_MainType;
	PokemonType::Types m_SubType;

	std::string m_PokemonName;
	int m_IndexPreEvo;
	int m_IndexProEvo;
	int m_LevelUp;

	//Stats
	float m_HP;
	float m_Attack;
	float m_Defense;
	float m_SpAttack;
	float m_SpDefense;
	float m_Speed;
	float m_Total;

	//gender
	float m_GenderRatioMale;
};

