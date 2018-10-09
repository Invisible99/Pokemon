#include "stdafx.h"
#include "Enemie.h"
#include "PokemonManager.h"
Enemie::Enemie(Point2f startPos, Point2f shape, int length, int gemLevel)
	: m_SpritesTexture{ "Resources/Images/Npc.png" }
	 , m_TextboxTexture{ "Resources/Images/Textbox.png" }
	 , m_StartPos{ startPos }
	 , m_Shape{Rectf( shape.x, shape.y, m_ClipWidth , m_ClipHeight)}
	 , m_GemLevel{ gemLevel }
{
	m_pFontText = TTF_OpenFont("Resources/Font/pkmnfl.ttf", 12);
	CreateRandomTeam(length);
	m_LookingWay = Down;
}

void Enemie::Update(float elapsedSec, Rectf & charShape, bool& isTalking, Vector2f& actorVelocity)
{
	if (!m_HasBattled) {
		if (!m_IsTalking) {
			ChangeLookingDirection(elapsedSec);
			CheckForBattle(charShape, isTalking);
		}
	}	
	CheckTrainer(charShape, actorVelocity);
}

void Enemie::Draw(Point2f& camPos) const
{
	glPushMatrix();
	if (m_IsTalking) {
		DrawTextbox(camPos);
	}
	switch (m_LookingWay)
	{
	case Right:
		glTranslatef(m_Shape.left + m_ClipWidth / 2.0f, 0.0f, 0.0f);
		glScalef(-1.0f, 1.0f, 1.0f);
		glTranslatef(-m_Shape.left - m_ClipWidth / 2, 0.0f, 0.0f);
		break;
	}

	m_SpritesTexture.Draw(m_Shape, Rectf{ m_StartPos.x, m_StartPos.y, m_ClipWidth, m_ClipHeight });

	glPopMatrix();
	
}

void Enemie::DrawTextbox(Point2f& camPos) const
{
	m_TextboxTexture.Draw(Rectf(camPos.x, camPos.y, m_TextboxWidth, m_TextboxHeight), Rectf(0, 0, m_TextboxWidth, m_TextboxHeight));
	std::unique_ptr<Texture> labelText = std::make_unique<Texture>("I see you! Let's fight!", m_pFontText, Color4f(0, 0, 0, 1));
	labelText->Draw(Point2f(camPos.x + 15, camPos.y + 15));
}

void Enemie::ChangeLookingDirection(float elapsedSec)
{
	m_SecondsPast += elapsedSec;

	if (m_SecondsPast > 5) {
		int rng = rand() % 4;
		m_LookingWay = static_cast<Looking>(rng);
		switch (m_LookingWay)
		{
		case Down:
			m_StartPos.x = 1.0f;
			break;
		case Up:
			m_StartPos.x = 16.0f;
			break;
		case Left:
			m_StartPos.x = 31.0f;
			break;
		case Right:
			m_StartPos.x = 31.0f;
			break;
		}
		m_SecondsPast = 0;
	}
}

void Enemie::CreateRandomTeam(int length)
{
	for (int i{ 0 }; i < length; ++i) {
		m_pPokemons.push_back(std::make_shared<TeamPokemon>(PokemonManager::GetInstance().GetPokemon(rand() % 146), rand() % m_GemLevel + (m_GemLevel /2)));
	}
}

void Enemie::CheckForBattle(Rectf & charShape, bool& isTalking)
{
	switch (m_LookingWay)
	{
	case Down:
		if (charShape.bottom + charShape.height >= m_Shape.bottom - 32 && charShape.bottom + charShape.height <= m_Shape.bottom && charShape.left + charShape.width >= m_Shape.left + m_Shape.width / 2 && charShape.left <= m_Shape.left + m_Shape.width - m_Shape.width/2) {
			InitiatBattle(isTalking);
		}
		break;
	case Up:
		if (charShape.bottom >= m_Shape.bottom + m_Shape.height && charShape.bottom<= m_Shape.bottom + m_Shape.height + 32 && charShape.left + charShape.width >= m_Shape.left + m_Shape.width / 2 && charShape.left <= m_Shape.left + m_Shape.width - m_Shape.width / 2) {
			InitiatBattle(isTalking);
		}
		break;
	case Left:
		if(charShape.left + charShape.width <= m_Shape.left && charShape.left + charShape.width >= m_Shape.left-32 && charShape.bottom + charShape.height >= m_Shape.bottom + m_Shape.height / 2 && charShape.bottom <= m_Shape.bottom + m_Shape.height - m_Shape.height / 2){
			InitiatBattle(isTalking);
		}
		break;
	case Right:
		if (charShape.left >= m_Shape.left + m_Shape.width && charShape.left <= m_Shape.left + m_Shape.width + 32 && charShape.bottom + charShape.height >= m_Shape.bottom + m_Shape.height / 2 && charShape.bottom <= m_Shape.bottom + m_Shape.height - m_Shape.height / 2) {
			InitiatBattle(isTalking);
		}
		break;
	}
}

void Enemie::InitiatBattle(bool& isTalking)
{
	m_IsTalking = true;
	isTalking = true;
}

void Enemie::CheckTrainer(Rectf& actorShape, Vector2f& actorVelocity)
{
	if (utils::IsOverlapping(m_Shape, actorShape)) {
		if (actorVelocity.x > 0) {//rechts
			actorShape.left = m_Shape.left - actorShape.width -0.1f;

		}
		else if (actorVelocity.x < 0) {//links
			actorShape.left = m_Shape.left + m_Shape.width + 0.1f;
		}
		else if (actorVelocity.y > 0) {//Up
			actorShape.bottom = m_Shape.bottom - actorShape.height - 0.1f;
		}
		else if (actorVelocity.y < 0) {//Down
			actorShape.bottom = m_Shape.bottom + m_Shape.height + 0.1f;
		}
		actorVelocity.x = 0;
		actorVelocity.y = 0;
		return;
	}
}

void Enemie::CheckToStartBattle(Battle& battle, std::vector<std::shared_ptr<TeamPokemon>>& charTeam, bool& isTalking)
{
	const Uint8 *pKeysStates = SDL_GetKeyboardState(nullptr);
	if (m_IsTalking) {
		if (pKeysStates[SDL_SCANCODE_SPACE]) {
			m_IsTalking = false;
			isTalking = false;
			m_HasBattled = true;
			battle.StartBattle(charTeam, m_pPokemons, true);
		}
	}

}
