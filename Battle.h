#pragma once
#include "TeamPokemon.h"
#include "SoundEffect.h"

class Battle
{
public:
	enum Choose {
		LeftUp,
		RightUp,
		LeftBottom,
		RightBottom,
	};
	explicit Battle(float scale);

	void StartBattle(std::vector<std::shared_ptr<TeamPokemon>>& team, std::vector<std::shared_ptr<TeamPokemon>>& enemyTeam, bool trainer);
	void StartBattle(std::vector<std::shared_ptr<TeamPokemon>>& team, std::shared_ptr<TeamPokemon>& enemyTeam);

	void StopBattle();

	bool IsInBattle();
	bool IsTransition();

	void Draw() const;
	void Update(float elapsedSec);

	void ButtonPresses(const Uint8 *pKeysStates);

	void AnimateTransition(float& scale, float elapsedSec);

	void CloseLabelFont();


private:	
	Texture m_BattleTexture;
	Rectf m_Background;
	Point2f m_TextboxPos;
	Point2f m_MoveboxPos;
	Point2f m_ChoosePos;
	Point2f m_HealthbarPos;
	Point2f m_HealthbarEnemyPos;
	Point2f m_PokSpritePos;
	Point2f m_EnemyPokSpritePos;
	Rectf m_Arrow;

	TTF_Font* m_pFontText;

	float m_Width;
	float m_Height;
	float m_Scale;

	float m_WidthHealthbar{49};
	float m_HeightHealthbar{ 5 };

	Choose m_Choose;
	int m_MoveChoose;

	float m_Distances{ 10.0f };
	float m_Speed{ 5.0f };
	float m_ToAnimatiePos;

	bool m_CharPokAttackedFirst;
	bool m_AttackChooses;
	bool m_IsCharPokemonAttacking{ false };
	bool m_IsEnemyPokemonAttacking{ false };
	bool m_IsUp{ false };
	float m_IsUpAmountOfSec;
	float m_SpeedUpDown;
	int m_TimesRunAway{0};
	
	bool m_InBattle;
	bool m_IsTransition;
	bool m_IsTrainer;
	
	std::vector<std::shared_ptr<TeamPokemon>> m_pCharPokemons;
	std::vector<std::shared_ptr<TeamPokemon>> m_pEnemyPokemons;
	int m_SelectedPokemon;
	int m_EnemySelectedPokemon;

	SoundEffect m_EffectAttack;

	void DrawArrow() const;
	void DrawPokemons() const;
	void DrawPokemonSprite() const;
	void DrawHealthBar(std::shared_ptr<TeamPokemon> p, Point2f startPos) const;
	void DrawMoves() const;
	void DrawMoveStat() const;	

	void MoveBetweenOptions(const Uint8 *pKeysStates);
	void MoveBetweenAttacks(const Uint8 *pKeysStates);

	void SelectStartPokemon();
	void HandleAttacks();
	void HandleTeamAttack();
	void HandleEnemyAttack();

	void HandleAnimation(float elapsedSec);
	void AnimateAttack(float elapsedSec, bool first);
	void AnimateEnemyAttack(float elapsedSec, bool first);

	bool IsTeamAlive();
	bool IsPokemonAlive();
	bool IsEnemyTeamAlive();
	bool IsEnemyPokemonAlive();
};

