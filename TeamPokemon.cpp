#include "stdafx.h"
#include "TeamPokemon.h"
#include "PokemonManager.h"
#include "Evolve.h"

TeamPokemon::TeamPokemon(std::shared_ptr<Pokemon> pokemon, int level)
	: m_pPokemon(pokemon),
	m_Level(level)
{
	SetLearnedMoves();
	m_ExpLevel = CalculateNextExp();
	CalculateStats();
}

void TeamPokemon::SetLearnedMoves() {
	if (m_pPokemon->GetIndexPreEvo() != -1) {
		std::shared_ptr<Pokemon> prevPok{ PokemonManager::GetInstance().GetPokemon(m_pPokemon->GetIndexPreEvo()) };
		if (prevPok->GetIndexPreEvo() != -1) {
			std::shared_ptr<Pokemon> firstEvo{ PokemonManager::GetInstance().GetPokemon(prevPok->GetIndexPreEvo()) };
			AddLearnedMoves(firstEvo);
		}
		AddLearnedMoves(prevPok);
	}
	AddLearnedMoves(m_pPokemon);
}

void TeamPokemon::CalculateStats()
{
	m_HPMax = ((2 * m_pPokemon->GetHP())*m_Level / 100) + m_Level + 10;
	m_HP = m_HPMax;
	m_Attack = ((2 * m_pPokemon->GetAttack())*m_Level / 100) + 5;
	m_Defense = ((2 * m_pPokemon->GetDefense())*m_Level / 100) + 5;
	m_SpAttack = ((2 * m_pPokemon->GetSpAttack())*m_Level / 100) + 5;
	m_SpDefense = ((2 * m_pPokemon->GetSpDefense())*m_Level / 100) + 5;
	m_Speed = ((2 * m_pPokemon->GetSpeed())*m_Level / 100) + 5;
}

void TeamPokemon::AddLearnedMoves(std::shared_ptr<Pokemon>& p) {
	bool excist{ false };

	for (size_t i{ 0 }; i < p->GetMoveset().size(); ++i) {
		if (p->GetLevelLearned()[i] <= m_Level) {
			if (m_pLearnedMoves.size() >= 4) {
				m_pLearnedMoves.pop_front();
			}
			for (size_t j{ 0 }; j < m_pLearnedMoves.size(); ++j) {
				if (p->GetName() == m_pLearnedMoves[j]->GetMove()->GetName()) {
					excist = true;
				}
			}
			if (!excist) {
				m_pLearnedMoves.push_back(std::make_shared<KnowMove>(p->GetMoveset()[i]));
			}
			else {
				excist = false;
			}			
		}
		else {
			return;
		}
	}
}

int TeamPokemon::GetLearnedMovesSize() {
	return m_pLearnedMoves.size();
}

std::shared_ptr<KnowMove> TeamPokemon::GetLearnedMove(int id) {
	return m_pLearnedMoves[id];
}

float TeamPokemon::GetPercentageHp() {
	return m_HP / m_HPMax;
}

void TeamPokemon::TakeDamage(float damage) {
	m_HP -= damage;
	if (m_HP < 0) {
		m_HP = 0;
	}
}

int TeamPokemon::GetLevel() {
	return m_Level;
}

float TeamPokemon::GetHP() {
	return m_HP;
}

float TeamPokemon::GetMaxHP() {
	return m_HPMax;
}

float TeamPokemon::GetAttack()
{
	return m_Attack;
}

float TeamPokemon::GetSpAttack()
{
	return m_SpAttack;
}

float TeamPokemon::GetDefense()
{
	return m_Defense;
}

float TeamPokemon::GetSpDefense()
{
	return m_SpDefense;
}

float TeamPokemon::GetSpeed()
{
	return m_Speed;
}

std::shared_ptr<Pokemon>& TeamPokemon::GetPokemonptr()
{
	return m_pPokemon;
}

void TeamPokemon::GainExp(std::shared_ptr<TeamPokemon>& enemyPokemon)
{
	m_ExpGet += ((rand() % 255 + 1) * enemyPokemon->GetLevel() * m_Level) / 7;

	while (m_ExpGet >= m_ExpLevel) {
		m_ExpGet = m_ExpGet - m_ExpLevel;
		LevelUp();
	}
}

void TeamPokemon::LevelUp()
{
	++m_Level;
	CalculateStats();
	m_pLearnedMoves.clear();
	SetLearnedMoves();
	if (m_Level < 100) {
		m_ExpLevel = CalculateNextExp();
		if (m_Level == m_pPokemon->GetLevelUp()) {
			Evolve::GetInstance().StartEvolving(m_pPokemon);
			m_pPokemon = PokemonManager::GetInstance().GetPokemon(m_pPokemon->GetIndexProEvo());
		}
	}
}

int TeamPokemon::CalculateNextExp()
{
	return int(((4 * pow((m_Level + 1), 3)) / 5) - ((4 * pow((m_Level), 3)) / 5));
}
