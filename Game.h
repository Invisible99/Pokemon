#pragma once
#include "MoveManager.h"
#include "PokemonManager.h"
#include "Character.h"
#include "Camera.h"
#include "Level.h"
#include "Battle.h"
#include "Enemie.h"


class Game
{
public:
	explicit Game( const Window& window );
	Game( const Game& other ) = delete;
	Game& operator=( const Game& other ) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw();

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	Window m_Window;

	float m_Scale;

	Character m_Char;
	Camera m_Cam;
	Level m_Level;
	Battle m_Battle;
	std::vector<std::shared_ptr<Enemie>> m_pEnemies;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( );

	void SetEnemies();
};