#include "stdafx.h"
#include "Game.h"
#include "Evolve.h"
#include "AudioManager.h"

Game::Game( const Window& window ) 
	:m_Window{ window }
	, m_Scale{2.0f}
	, m_Cam{ Camera(480,320) }
	, m_Battle{m_Scale}
{
	Initialize( );
	SetEnemies();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Cam.SetBoundaries(m_Level.GetBoundaries());
	MoveManager::GetInstance().LoadMoves();
	PokemonManager::GetInstance().LoadPokemons();
	m_Char.CreateRandomTeam();
	AudioManager::GetInstance().SetVolume(10);
	AudioManager::GetInstance().PlayOverworld();
}

void Game::Cleanup( )
{
	delete &MoveManager::GetInstance();
	delete &PokemonManager::GetInstance();
	delete &PokemonType::GetInstance();
	delete &Evolve::GetInstance();
	delete &AudioManager::GetInstance();
	m_Battle.CloseLabelFont();
}

void Game::Update( float elapsedSec )
{
	if (!Evolve::GetInstance().GetEvolving()) {
		if (!m_Char.IsTalking()) {
			if (!m_Battle.IsInBattle()) {
				if (!m_Battle.IsTransition()) {
					m_Char.Update(elapsedSec, m_Level, m_Battle);
				}
				else {
					m_Battle.AnimateTransition(m_Scale, elapsedSec);
				}
				
				for (size_t i{ 0 }; i < m_pEnemies.size(); ++i) {
					m_pEnemies[i]->Update(elapsedSec, m_Char.GetShape(), m_Char.IsTalking(), m_Char.GetVelocity());
				}
			}
			else {
				m_Battle.Update(elapsedSec);
			}
		}	
	}
	else {
		Evolve::GetInstance().Update(elapsedSec);
	}
}

void Game::Draw()
{
	ClearBackground( );

	glPushMatrix();
	glScalef(m_Scale, m_Scale, 1.0f);
	if (!Evolve::GetInstance().GetEvolving()) {
		if (!m_Battle.IsInBattle()) {
			glTranslatef(floor(-m_Cam.GetPosition(m_Char.GetShape(), m_Scale).x), floor(-m_Cam.GetPosition(m_Char.GetShape(), m_Scale).y), 0);
			m_Level.DrawBackground();
			m_Char.Draw();
			for (size_t i{ 0 }; i < m_pEnemies.size(); ++i) {
				m_pEnemies[i]->Draw(m_Cam.GetPosition(m_Char.GetShape(), m_Scale));
			}
		}
		else {
			m_Battle.Draw();
		}
	}
	else {
		Evolve::GetInstance().Draw();
	}
	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	const Uint8 *pKeysStates = SDL_GetKeyboardState(nullptr);
	if (m_Battle.IsInBattle()) {
		m_Battle.ButtonPresses(pKeysStates);
	}
	if (pKeysStates[SDL_SCANCODE_B]) {
		int gemLevel{ 0 };
		for (size_t i{ 0 }; i < m_Char.GetPokemons().size(); ++i) {
			gemLevel += m_Char.GetPokemons()[i]->GetLevel();
		}
		gemLevel = gemLevel / m_Char.GetPokemons().size();
		std::shared_ptr<TeamPokemon> enemyTeam{ std::make_shared<TeamPokemon>(PokemonManager::GetInstance().GetPokemon(rand() % 146),rand() % gemLevel+ (gemLevel / 2)) };
		m_Battle.StartBattle(m_Char.GetPokemons(), enemyTeam);
	}
	if (m_Char.IsTalking()) {
		for (size_t i{ 0 }; i < m_pEnemies.size(); ++i) {
			m_pEnemies[i]->CheckToStartBattle(m_Battle, m_Char.GetPokemons(), m_Char.IsTalking());
		}		
	}

	if (pKeysStates[SDL_SCANCODE_KP_PLUS]) {
		AudioManager::GetInstance().SetVolume(AudioManager::GetInstance().GetVolume() + 8);
	}
	if (pKeysStates[SDL_SCANCODE_KP_MINUS]) {
		AudioManager::GetInstance().SetVolume(AudioManager::GetInstance().GetVolume() - 8);
	}

	if (pKeysStates[SDL_SCANCODE_O]) {
		Evolve::GetInstance().StartEvolving(m_Char.GetPokemons()[0]->GetPokemonptr());
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{

}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::SetEnemies()
{
	int gemLevel{ 0 };
	for (size_t i{ 0 }; i < m_Char.GetPokemons().size(); ++i) {
		gemLevel += m_Char.GetPokemons()[i]->GetLevel();
	}
	gemLevel = gemLevel / m_Char.GetPokemons().size();
	m_pEnemies.push_back(std::make_shared<Enemie>(Point2f(1, 21), Point2f(192, 124), 1, gemLevel));
	m_pEnemies.push_back(std::make_shared<Enemie>(Point2f(1, 41), Point2f(259, 212), 2, gemLevel));
	m_pEnemies.push_back(std::make_shared<Enemie>(Point2f(1, 62), Point2f(638, 113), 2, gemLevel));
	m_pEnemies.push_back(std::make_shared<Enemie>(Point2f(1, 83), Point2f(1020, 73), 3, gemLevel));
}
