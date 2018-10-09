#pragma once
#include "pokemon.h"
#include "KnowMove.h"
class TeamPokemon
{
public:
	explicit TeamPokemon(std::shared_ptr<Pokemon> pokemon, int level);

	void TakeDamage(float damage);
	void GainExp(std::shared_ptr<TeamPokemon>& enemyPokemon);

	float GetPercentageHp();
	int GetLearnedMovesSize();
	int GetLevel();
	float GetHP();
	float GetMaxHP();
	float GetAttack();
	float GetSpAttack();
	float GetDefense();
	float GetSpDefense();
	float GetSpeed();
	std::shared_ptr<Pokemon>& GetPokemonptr();
	std::shared_ptr<KnowMove> GetLearnedMove(int id);

private:
	std::shared_ptr<Pokemon> m_pPokemon;
	std::deque<std::shared_ptr<KnowMove>> m_pLearnedMoves;

	int m_Level;
	int m_ExpGet{ 0 };
	int m_ExpLevel;

	//Stats
	float m_HP;
	float m_HPMax;
	float m_Attack;
	float m_Defense;
	float m_SpAttack;
	float m_SpDefense;
	float m_Speed;

	void CalculateStats();
	void SetLearnedMoves();
	void AddLearnedMoves(std::shared_ptr<Pokemon>& p);

	void LevelUp();
	int CalculateNextExp();
};

