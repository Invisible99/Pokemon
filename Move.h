#pragma once
#include "PokemonType.h"
#include "StatusEffects.h"
#include <string>
class Move
{
public:	
	enum AttackCategory {
		Physical,
		Special,
		StatusCat
	};

	enum AttackEffect {
		Normal,
		StatusEff,
		Double
	};

	explicit Move(std::string name, PokemonType::Types attackType, AttackCategory categoryAttack, 
		AttackEffect effectAttack, StatusEffects::AttackStatus status, int maxAmount, 
		float power, float accuracy, bool m_tm);

	float CalculateAttack(int level, float attPok, float spattPok, float defPok, float spdefPok);

	std::string GetName();

	int GetPPMax();
	int GetAttackType();

private:
	std::string m_Name;
	PokemonType::Types m_AttackType;

	AttackCategory m_CategoryAttack;
	AttackEffect m_EffectAttack;
	StatusEffects::AttackStatus m_Status;

	int m_PPMax;
	float m_Power;
	float m_Accuracy;
	bool m_TM;	
};

