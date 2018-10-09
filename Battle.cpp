#include "stdafx.h"
#include "Battle.h"
#include "PokemonManager.h"
#include "MoveManager.h"
#include <algorithm>
#include "PokemonType.h"
#include "AudioManager.h"

Battle::Battle(float scale)
	: m_BattleTexture{ "Resources/Images/Battle.png"}
	, m_Background{Rectf(3.0f,232.0f,240.0f,112.0f)}
	, m_TextboxPos{Point2f(297.0f,103.0f)}
	, m_MoveboxPos{Point2f(297.0f,51.0f)}
	, m_ChoosePos{Point2f(147.0f,51.0f)}
	, m_HealthbarPos{ Point2f(172.0f,66.0f) }
	, m_HealthbarEnemyPos{ Point2f(49.0f, 128.0f) }
	, m_Arrow{ Rectf(268.0f, 13.0f, 7.0f, 10.0f) }
	, m_Width {240.f}
	, m_Height {48.0f}
	, m_Choose{LeftUp}
	, m_Scale{scale}
	, m_EffectAttack{ "Resources/Audio/Scratch.wav" }
	, m_PokSpritePos{ 25.0f, 48.0f }
	, m_EnemyPokSpritePos { 145.0f,90.0f }
	, m_pCharPokemons {}
	, m_pEnemyPokemons{}
{
	m_pFontText = TTF_OpenFont("Resources/Font/pkmnfl.ttf",12);
}

void Battle::StartBattle(std::vector<std::shared_ptr<TeamPokemon>>& team, std::vector<std::shared_ptr<TeamPokemon>>& enemyTeam, bool trainer) {
	m_IsTransition = true;
	m_IsTrainer = trainer;
	m_pCharPokemons = team;
	m_pEnemyPokemons = enemyTeam;

	SelectStartPokemon();
}

void Battle::StartBattle(std::vector<std::shared_ptr<TeamPokemon>>& team, std::shared_ptr<TeamPokemon>& enemyTeam) {
	m_InBattle = true;
	m_IsTrainer = false;
	m_pCharPokemons = team;
	m_pEnemyPokemons.push_back(enemyTeam);
	SelectStartPokemon();
}

void Battle::StopBattle()
{
	m_pCharPokemons.clear();
	m_pEnemyPokemons.clear();
	m_AttackChooses = false;
	m_InBattle = false;
	m_IsCharPokemonAttacking = false;
	m_IsEnemyPokemonAttacking = false;
	AudioManager::GetInstance().PlayOverworld();
}

bool Battle::IsInBattle() {
	return m_InBattle;
}

bool Battle::IsTransition()
{
	return m_IsTransition;
}

void Battle::Draw() const {
	m_BattleTexture.Draw(Rectf(0.0f, 48.0f, m_Width, m_Background.height), m_Background);
	DrawPokemons();
	if (!m_AttackChooses) {
		m_BattleTexture.Draw(Rectf(0.0f, 0.0f, m_Width, m_Height), Rectf(m_TextboxPos.x, m_TextboxPos.y, m_Width, m_Height));
		m_BattleTexture.Draw(Rectf(m_Width / 2.0f, 0.0f, m_Width/2.0f, m_Height), Rectf(m_ChoosePos.x, m_ChoosePos.y, m_Width/2.0f, m_Height));
	}
	else {
		m_BattleTexture.Draw(Rectf(0.0f, 0.0f, m_Width, m_Height), Rectf(m_MoveboxPos.x, m_MoveboxPos.y, m_Width, m_Height));
		DrawMoves();
		DrawMoveStat();
	}	
	DrawArrow();
}

void Battle::Update(float elapsedSec) {
	if (m_IsCharPokemonAttacking) {
		AnimateAttack(elapsedSec, m_CharPokAttackedFirst);
	}
	else {
		if (!m_IsEnemyPokemonAttacking) {
			HandleAnimation(elapsedSec);
		}
	}
	if (m_IsEnemyPokemonAttacking) {
		AnimateEnemyAttack(elapsedSec, !m_CharPokAttackedFirst);
	}
}

void Battle::ButtonPresses(const Uint8 *pKeysStates) {
	if (!m_IsCharPokemonAttacking && !m_IsEnemyPokemonAttacking) {
		if (!m_AttackChooses) {
			MoveBetweenOptions(pKeysStates);
		}
		else {
			MoveBetweenAttacks(pKeysStates);
		}

		if (pKeysStates[SDL_SCANCODE_Q]) {
			if (!m_AttackChooses) {
				switch (m_Choose)
				{
				case LeftUp:
					if (!m_AttackChooses) {
						m_AttackChooses = true;
					}
					break;
				case RightUp:
					// bag
					break;
				case LeftBottom:
					// open pokemon choose
					break;
				case RightBottom:
					if (!m_AttackChooses) {
						if (!m_IsTrainer) {
							StopBattle();
						}						
					}
					break;
				default:
					break;
				}
			}
			else {
				HandleAttacks();
			}
		}

		if (pKeysStates[SDL_SCANCODE_E]) {
			if (m_AttackChooses) {
				m_AttackChooses = false;
				m_Choose = LeftUp;

			}
		}
	}
}

void Battle::AnimateTransition(float & scale, float elapsedSec)
{
	scale += 0.2f;

	if (scale >= 12.0f) {
		scale = 2.0f;
		m_IsTransition = false;
		m_InBattle = true;
	}
}

void Battle::MoveBetweenOptions(const Uint8 *pKeysStates) {
	if (pKeysStates[SDL_SCANCODE_W] || pKeysStates[SDL_SCANCODE_S]) {
		switch (m_Choose)
		{
		case LeftUp:
			m_Choose = LeftBottom;
			break;
		case RightUp:
			m_Choose = RightBottom;
			break;
		case LeftBottom:
			m_Choose = LeftUp;
			break;
		case RightBottom:
			m_Choose = RightUp;
			break;
		}
	}
	else if (pKeysStates[SDL_SCANCODE_D] || pKeysStates[SDL_SCANCODE_A]) {
		switch (m_Choose)
		{
		case LeftUp:
			m_Choose = RightUp;
			break;
		case RightUp:
			m_Choose = LeftUp;
			break;
		case LeftBottom:
			m_Choose = RightBottom;
			break;
		case RightBottom:
			m_Choose = LeftBottom;
			break;
		}
	}
}

void Battle::MoveBetweenAttacks(const Uint8 *pKeysStates) {
	switch (m_pCharPokemons[m_SelectedPokemon]->GetLearnedMovesSize()) {
		case 2:
			if (pKeysStates[SDL_SCANCODE_D] || pKeysStates[SDL_SCANCODE_A]) {
				switch (m_Choose)
				{
				case LeftUp:
					m_Choose = RightUp;
					break;
				case RightUp:
					m_Choose = LeftUp;
					break;
				}
			}
			break;
		case 3:
			if (pKeysStates[SDL_SCANCODE_D] || pKeysStates[SDL_SCANCODE_A]) {
				switch (m_Choose)
				{
				case LeftUp:
					m_Choose = RightUp;
					break;
				case RightUp:
					m_Choose = LeftUp;
					break;
				case LeftBottom:
					m_Choose = RightUp;
					break;
				}			
			}
			if (pKeysStates[SDL_SCANCODE_W] || pKeysStates[SDL_SCANCODE_S]) {
				switch (m_Choose)
				{
				case LeftUp:
					m_Choose = LeftBottom;
					break;
				case RightUp:
					m_Choose = LeftBottom;
					break;
				case LeftBottom:
					m_Choose = LeftUp;
					break;
				}
			}
			break;
		case 4:
			MoveBetweenOptions(pKeysStates);
			break;
	}
}

void Battle::DrawArrow() const {
	Point2f p{};
	if (!m_AttackChooses) {
		switch (m_Choose)
		{
		case LeftUp:
			p = Point2f(125, 25);
			break;
		case RightUp:
			p = Point2f(180, 25.0);
			break;
		case LeftBottom:
			p = Point2f(125.0, 10.0);
			break;
		case RightBottom:
			p = Point2f(180.0, 10.0);
			break;
		}
	}
	else {
		switch (m_Choose)
		{
		case LeftUp:
			p = Point2f(10.0, 25.0);
			break;
		case RightUp:
			p = Point2f(80.0, 25.0);
			break;
		case LeftBottom:
			p = Point2f(10.0, 10.0);
			break;
		case RightBottom:
			p = Point2f(80.0, 10.0);
			break;
		}
	}
	m_BattleTexture.Draw(Rectf(p.x, p.y, m_Arrow.width, m_Arrow.height), m_Arrow);
}

void Battle::DrawPokemons() const{	
	m_BattleTexture.Draw(Rectf(10.0f, 120.0f, 100.0f, 30.0f), Rectf(3.0f, 31.0f, 100.0f, 30.0f));

	PokemonManager::GetInstance().GetSpriteSheet()->Draw(Rectf(m_EnemyPokSpritePos.x, m_EnemyPokSpritePos.y, PokemonManager::GetInstance().GetWidth(), PokemonManager::GetInstance().GetHeight()),
		Rectf(m_pEnemyPokemons[m_EnemySelectedPokemon]->GetPokemonptr()->GetStartPos().x, m_pEnemyPokemons[m_EnemySelectedPokemon]->GetPokemonptr()->GetStartPos().y, PokemonManager::GetInstance().GetWidth(), PokemonManager::GetInstance().GetHeight()));
	DrawHealthBar(m_pEnemyPokemons[m_EnemySelectedPokemon], m_HealthbarEnemyPos);

	std::string nameEnemy = m_pEnemyPokemons[m_EnemySelectedPokemon]->GetPokemonptr()->GetName();
	std::transform(nameEnemy.begin(), nameEnemy.end(), nameEnemy.begin(), toupper);
	std::unique_ptr<Texture> labelText = std::make_unique<Texture>(nameEnemy, m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(15.0f , 135.0f));

	labelText = std::make_unique<Texture>(std::to_string(m_pEnemyPokemons[m_EnemySelectedPokemon]->GetLevel()), m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(92.0f , 135.0f));
	
	DrawPokemonSprite();

	std::string name = m_pCharPokemons[m_SelectedPokemon]->GetPokemonptr()->GetName();
	std::transform(name.begin(), name.end(), name.begin(), toupper);
	labelText = std::make_unique<Texture>(name, m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(140.0f, 73.0f));

	labelText = std::make_unique<Texture>(std::to_string(m_pCharPokemons[m_SelectedPokemon]->GetLevel()), m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(217.0f, 73.0f));
}

void Battle::DrawPokemonSprite() const
{
	m_BattleTexture.Draw(Rectf(125.0f, 50.0f, 104.0f, 37.0f), Rectf(3.0f, 80.0f, 104.0f, 37.0f));
	PokemonManager::GetInstance().GetSpriteSheet()->Draw(Rectf(m_PokSpritePos.x, m_PokSpritePos.y, PokemonManager::GetInstance().GetWidth(), PokemonManager::GetInstance().GetHeight()),
		Rectf(m_pCharPokemons[m_SelectedPokemon]->GetPokemonptr()->GetStartPos().x + PokemonManager::GetInstance().GetWidth(), m_pCharPokemons[m_SelectedPokemon]->GetPokemonptr()->GetStartPos().y, PokemonManager::GetInstance().GetWidth(), PokemonManager::GetInstance().GetHeight()));
	DrawHealthBar(m_pCharPokemons[m_SelectedPokemon], m_HealthbarPos);
}

void Battle::DrawHealthBar(std::shared_ptr<TeamPokemon> p, Point2f startPos) const {
	float hp = p->GetPercentageHp();
	Rectf source{};
	if (hp > 0) {
		if (hp > 0.5) {
			source = Rectf(117.0f, 11.0f, 9.0f, 3.0f);
		}
		else if (hp >0.1) {
			source = Rectf(117.0f, 15.0f, 9.0f, 3.0f);
		}
		else {
			source = Rectf(117.0f, 19.0f, 9.0f, 3.0f);
		}
		m_BattleTexture.Draw(Rectf(startPos.x, startPos.y, m_WidthHealthbar*hp, m_HeightHealthbar), source);
	}
}

void Battle::DrawMoves() const {
	int size = m_pCharPokemons[m_SelectedPokemon]->GetLearnedMovesSize();

	Point2f p{ Point2f(10, 25) };
	int counter{ 0 };
	for (int i{ 0 }; i < m_pCharPokemons[m_SelectedPokemon]->GetLearnedMovesSize(); ++i) {
		std::string move = m_pCharPokemons[m_SelectedPokemon]->GetLearnedMove(i)->GetMove()->GetName();
		std::transform(move.begin(), move.end(), move.begin(), toupper);
		std::unique_ptr<Texture> labelText = std::make_unique<Texture>(move, m_pFontText, Color4f(0, 0, 0, 1));
		labelText->Draw(Point2f((p.x + (counter * 70) + m_Arrow.width), (p.y)));
		++counter;
		if (i == 1) {
			p.y = 10;
			counter = 0;
		}
	}
}

void Battle::DrawMoveStat() const {
	std::unique_ptr<Texture> labelText = std::make_unique<Texture>(std::to_string(m_pCharPokemons[m_SelectedPokemon]->GetLearnedMove(m_Choose)->GetPP()), m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(200, 27));

	labelText = std::make_unique<Texture>(std::to_string(m_pCharPokemons[m_SelectedPokemon]->GetLearnedMove(m_Choose)->GetMove()->GetPPMax()), m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(220, 27));
	
	std::weak_ptr<std::string> moveType = PokemonType::GetInstance().GetTypeText(m_pCharPokemons[m_SelectedPokemon]->GetLearnedMove(m_Choose)->GetMove()->GetAttackType());
	std::transform(moveType.lock()->begin(), moveType.lock()->end(), moveType.lock()->begin(), toupper);
	labelText = std::make_unique<Texture>(moveType.lock()->data(), m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(192, 10));
}

void Battle::AnimateAttack(float elapsedSec, bool first)
{
	m_PokSpritePos.x += m_Distances * m_Speed * elapsedSec;
	if (m_PokSpritePos.x >= m_ToAnimatiePos)
	{
		m_EffectAttack.Play(0);
		std::shared_ptr<KnowMove> attackMove = m_pCharPokemons[m_SelectedPokemon]->GetLearnedMove(m_Choose);

		attackMove->SetPP(attackMove->GetPP() - 1);
		m_pEnemyPokemons[m_EnemySelectedPokemon]->TakeDamage(attackMove->GetMove()->
			CalculateAttack(m_pCharPokemons[m_SelectedPokemon]->GetLevel(), m_pCharPokemons[m_SelectedPokemon]->GetAttack(), m_pCharPokemons[m_SelectedPokemon]->GetSpAttack(), m_pEnemyPokemons[m_EnemySelectedPokemon]->GetDefense(), m_pEnemyPokemons[m_EnemySelectedPokemon]->GetSpDefense()));

		m_IsCharPokemonAttacking = false;
		m_PokSpritePos = Point2f(25.0f, 48.0f);
			
		if (first) {
			if (IsEnemyPokemonAlive()) {
				HandleEnemyAttack();
			}
		}
		if (!IsEnemyTeamAlive()) {
			m_pCharPokemons[m_SelectedPokemon]->GainExp(m_pEnemyPokemons[m_EnemySelectedPokemon]);
			StopBattle();
			return;
		}
	}
}

void Battle::AnimateEnemyAttack(float elapsedSec, bool first){
	m_EnemyPokSpritePos.x -= m_Distances * m_Speed * elapsedSec;
	if (m_EnemyPokSpritePos.x <= m_ToAnimatiePos)
	{
		m_EffectAttack.Play(0);

		int rng;
		do {
			rng = rand() % m_pEnemyPokemons[m_EnemySelectedPokemon]->GetLearnedMovesSize();
		} while (m_pEnemyPokemons[m_EnemySelectedPokemon]->GetLearnedMove(rng)->GetPP() <= 0);

		m_pCharPokemons[m_SelectedPokemon]->TakeDamage(m_pEnemyPokemons[m_EnemySelectedPokemon]->GetLearnedMove(rng)->GetMove()->
			CalculateAttack(m_pEnemyPokemons[m_EnemySelectedPokemon]->GetLevel(), m_pEnemyPokemons[m_EnemySelectedPokemon]->GetAttack(), m_pEnemyPokemons[m_EnemySelectedPokemon]->GetSpAttack(), m_pCharPokemons[m_SelectedPokemon]->GetDefense(), m_pCharPokemons[m_SelectedPokemon]->GetSpDefense()));
		m_EnemyPokSpritePos = Point2f(145.0f, 90.0f);
		m_IsEnemyPokemonAttacking = false;

		if (first) {
			if (IsPokemonAlive()) {
				HandleTeamAttack();
			}
		}
		if (!IsTeamAlive()) {
			StopBattle();
			return;
		}
	}
}

void Battle::SelectStartPokemon()
{
	m_SelectedPokemon = -1;
	m_EnemySelectedPokemon = -1;
	for (size_t i = 0; i < m_pCharPokemons.size(); ++i)
	{
		if (m_pCharPokemons[i]->GetHP() > 0) {
			m_SelectedPokemon = i;
			break;
		}
	}
	for (size_t i = 0; i < m_pEnemyPokemons.size(); ++i)
	{
		if (m_pEnemyPokemons[i]->GetHP() > 0) {
			m_EnemySelectedPokemon = i;
			break;
		}
	}
	if (m_SelectedPokemon == -1 || m_EnemySelectedPokemon == -1) {
		StopBattle();
	}
	AudioManager::GetInstance().PlayBattle();
}

void Battle::HandleAttacks() {
	if (m_pCharPokemons[m_SelectedPokemon]->GetLearnedMove(m_Choose)->GetPP() > 0) {
		if (m_pCharPokemons[m_SelectedPokemon]->GetSpeed() > m_pEnemyPokemons[m_EnemySelectedPokemon]->GetSpeed()) {
			m_CharPokAttackedFirst = true;
			HandleTeamAttack();
		}
		else {
			m_CharPokAttackedFirst = false;
			HandleEnemyAttack();
		}
	}
	m_AttackChooses = false;
	m_Choose = LeftUp;
	if (!IsTeamAlive() || !IsEnemyTeamAlive()) {
		m_pCharPokemons[m_SelectedPokemon]->GainExp(m_pEnemyPokemons[m_EnemySelectedPokemon]);
		StopBattle();
	}
}

void Battle::HandleTeamAttack()
{
	m_IsCharPokemonAttacking = true;
	m_ToAnimatiePos = m_PokSpritePos.x + m_Distances;
}

void Battle::HandleEnemyAttack()
{
	m_IsEnemyPokemonAttacking = true;
	m_ToAnimatiePos = m_EnemyPokSpritePos.x - m_Distances;
}

void Battle::HandleAnimation(float elapsedSec)
{
	float hp = m_pCharPokemons[m_SelectedPokemon]->GetPercentageHp();

	if (hp > 0.5) {
		m_SpeedUpDown = 0.5f;

	}
	else if (hp > 0.1) {
		m_SpeedUpDown = 1.0f;

	}
	else {
		m_SpeedUpDown = 1.5f;

	}
	m_IsUpAmountOfSec += elapsedSec;
	if (m_IsUp) {
		m_PokSpritePos = Point2f(25.0f, 45.0f);
		if (m_IsUpAmountOfSec >= m_SpeedUpDown) {
			m_IsUp = false;
			m_IsUpAmountOfSec = 0;
		}
	}
	else {
		m_PokSpritePos = Point2f(25.0f, 48.0f);
		if (m_IsUpAmountOfSec >= m_SpeedUpDown) {
			m_IsUp = true;
			m_IsUpAmountOfSec = 0;
		}
	}
}

bool Battle::IsTeamAlive()
{
	for (size_t i = 0; i < m_pCharPokemons.size(); ++i)
	{
		if (m_pCharPokemons[i]->GetHP() > 0) {
			
			m_SelectedPokemon = i;
			return true;
		}
	}
	return false;
}

bool Battle::IsPokemonAlive()
{
	if (m_pCharPokemons[m_SelectedPokemon]->GetHP() > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Battle::IsEnemyTeamAlive()
{
	for (size_t i = 0; i < m_pEnemyPokemons.size(); ++i)
	{
		if (m_pEnemyPokemons[i]->GetHP() > 0) {
			
			m_EnemySelectedPokemon = i;
			return true;
		}
	}
	return false;
}

bool Battle::IsEnemyPokemonAlive()
{
	if (m_pEnemyPokemons[m_EnemySelectedPokemon]->GetHP() > 0) {
		return true;
	}
	else {
		return false;
	}
}

void Battle::CloseLabelFont()
{
	if (m_pFontText != nullptr)
	{
		TTF_CloseFont(m_pFontText);
		m_pFontText = nullptr;
	}
}