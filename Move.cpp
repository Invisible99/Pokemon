#include "stdafx.h"
#include "Move.h"

Move::Move(std::string name, PokemonType::Types attackType, AttackCategory categoryAttack, AttackEffect effectAttack, StatusEffects::AttackStatus status, int maxAmount, float power, float accuracy, bool tm)
	: m_Name{ name}
	, m_AttackType { attackType}
	, m_CategoryAttack{ categoryAttack }
	, m_EffectAttack{ effectAttack }
	, m_Status{ status }
	, m_PPMax{ maxAmount }
	, m_Power{ power }
	, m_Accuracy{ accuracy }
	, m_TM{ tm }

{
}

float Move::CalculateAttack(int level, float attPok, float spattPok, float defPok, float spdefPok) {
	float damage{ 0 };
	switch (m_CategoryAttack)
	{
	case Physical:
		damage = (((2 * level) / 5) + 2 * m_Power * (attPok / defPok)) / 50 + 2;
		break;
	case Special:
		damage = (((2 * level) / 5) + 2 * m_Power * (spattPok / spdefPok)) / 50 + 2;
		break;
	case StatusCat:
		damage = (((2 * level) / 5) + 2 * m_Power * (spattPok / spdefPok)) / 50 + 2;
		break;
	}
	return damage;
}

std::string Move::GetName() {
	return m_Name;
}

int Move::GetPPMax() {
	return m_PPMax;
}

int Move::GetAttackType() {
	return m_AttackType;
}

