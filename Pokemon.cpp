#include "stdafx.h"
#include "Pokemon.h"
#include "MoveManager.h"
#include "PokemonManager.h"
Pokemon::Pokemon(std::string name, int mainType, int subType, float hp, float att, float def, float spAttack, float spDef, float speed, float total, int preEvo, int proEvo, int level, float genderRatio,
	std::string soundFile, float xPos, float yPos)
	: m_PokemonName{ name }
	, m_MainType{PokemonType::Types(mainType)}
	, m_SubType{ PokemonType::Types(mainType) }
	, m_HP{ hp }
	, m_Attack{att}
	, m_Defense{def}
	, m_SpAttack {spAttack}
	, m_SpDefense{spDef}
    , m_Speed{ speed }
	, m_GenderRatioMale{genderRatio}
	, m_StartPos{Point2f(xPos,yPos)}
	, m_IndexPreEvo{preEvo}
	, m_IndexProEvo{proEvo}
	, m_LevelUp{level}
{
}

void Pokemon::AddMove(int level, int moveID) {
	m_pMoves.push_back(MoveManager::GetInstance().GetMove(moveID));
	m_LevelsLearned.push_back(level);
}

float Pokemon::GetHP() {
	return m_HP;
}

Point2f Pokemon::GetStartPos() {
	return m_StartPos;
}

std::string Pokemon::GetName() {
	return m_PokemonName;
}

float Pokemon::GetAttack()
{
	return m_Attack;
}

float Pokemon::GetSpAttack()
{
	return m_SpAttack;
}

float Pokemon::GetDefense()
{
	return m_Defense;
}
float Pokemon::GetSpDefense()
{
	return m_SpDefense;
}

float Pokemon::GetSpeed()
{
	return m_Speed;
}

int Pokemon::GetIndexPreEvo()
{
	return m_IndexPreEvo;
}

int Pokemon::GetIndexProEvo()
{
	return m_IndexProEvo;
}

int Pokemon::GetLevelUp()
{
	return m_LevelUp;
}

const std::vector<std::shared_ptr<Move>>& Pokemon::GetMoveset()
{
	return m_pMoves;
}

const std::vector<int>& Pokemon::GetLevelLearned()
{
	return m_LevelsLearned;
}
