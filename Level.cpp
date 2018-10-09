#include "stdafx.h"
#include "Split.h"
#include "Level.h"
#include <fstream>
#include <ostream>
#include <math.h>
#include <stdlib.h>  
#include <time.h>
#include "PokemonManager.h"

Level::Level()
	: m_TileMapWorld{ "Resources/Images/TileWorld.png" }
	, m_TileMapHouses{"Resources/Images/TileHouses.png"}

{
	LoadFile("Resources/Text/Route3_Map.csv", m_VectorBackground, true);
	LoadFile("Resources/Text/Route3_Unwalkable.csv", m_VectorForeground, false);
	LoadFile("Resources/Text/Route3_Grass.csv", m_VectorGrass, false);
	LoadFile("", m_VectorHouses, false);
	LoadFile("Resources/Text/Route3_Ledge.csv", m_VectorLedge, false);
	m_Boundaries = (Rectf(0, 0, m_TileWidth*m_MapWidth, m_TileHeight*m_MapHeight));
}

void Level::DrawBackground() {
	if (!m_VectorBackground.empty()) {
		DrawMap(m_VectorBackground);
	}
	if (!m_VectorGrass.empty()) {
		DrawMap(m_VectorGrass);
	}
	if (!m_VectorForeground.empty()) {
		DrawMap(m_VectorForeground);
	}
	if (!m_VectorHouses.empty()) {
		DrawHouses(m_VectorHouses);
	}
	if (!m_VectorLedge.empty()) {
		DrawMap(m_VectorLedge);
	}
}


void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool& ledgeFound, float& toEndLegde, float& fromBeginLegde) const{
	CheckUnwalkable(actorShape, actorVelocity);
	CheckLedge(actorShape, actorVelocity, ledgeFound, toEndLegde, fromBeginLegde);
	CheckHouse(actorShape, actorVelocity);
}

Rectf Level::GetBoundaries()
{
	return m_Boundaries;
}

bool & Level::IsLedgeFound()
{
	return m_LegdeFound;
}

void Level::LoadFile(std::string filename, std::vector<int>& fill, bool count) {
	if (filename != "") {

		std::ifstream moveFile{ filename };
		if (!moveFile) {
			std::cout << "Openen van bestand mislukt: " << filename << "\n";
			std::cin.get();
		}

		std::string lineString{""};
		std::string fullString{""};

		while (moveFile && std::getline(moveFile, lineString)) {
			
			fullString += lineString + ',';
			if (count) {
				++m_MapHeight;
				if (m_MapWidth == 0) {
					m_MapWidth = explodeInt(lineString, ',').size();
				}
			}
			
		}

		m_MapHeight;
		m_MapWidth;
		fill = explodeInt(fullString, ',');
	}
}

void Level::DrawMap(std::vector<int>& toDraw) const {
	for (int i{ 0 }; i < m_MapHeight; ++i) {
		for (int j{ 0 }; j < m_MapWidth; ++j) {
			if (toDraw[(m_MapWidth*i) + j] != -1) {
				float tilePlaced = floor(toDraw[((m_MapWidth)*i) + j] / m_TileColumnsWorld);
				float tilex = (toDraw[((m_MapWidth)*i) + j] - (tilePlaced *m_TileColumnsWorld)) * m_TileWidth + 1 * (toDraw[((m_MapWidth)*i) + j] - (tilePlaced *m_TileColumnsWorld)) + 1;
				float tiley = (tilePlaced + 1) * m_TileHeight + 1 * tilePlaced + 1;
				m_TileMapWorld.Draw(Rectf(floor(j*m_TileWidth), floor((m_MapHeight - (i+1))*m_TileHeight), m_TileWidth, m_TileHeight),
					Rectf(tilex, tiley, m_TileWidth, m_TileHeight));
			}
		}
	}
}

void Level::DrawHouses(std::vector<int>& toDraw) const {
	for (int i{ 0 }; i < m_MapHeight; ++i) {
		for (int j{ 0 }; j < m_MapWidth; ++j) {
			if (toDraw[((m_MapWidth)*i) + j] != -1) {
				int tilePlaced = (int)(toDraw[((m_MapWidth)*i) + j] / m_TileColumnsHouses);
				int tilex = (int)((toDraw[((m_MapWidth)*i) + j] - (tilePlaced *m_TileColumnsHouses)) * m_TileWidth + 1 * (toDraw[((m_MapWidth)*i) + j] - (tilePlaced *m_TileColumnsHouses)) + 1);
				int tiley = (int)((tilePlaced + 1) * m_TileHeight + 1 * tilePlaced + 1);
				m_TileMapHouses.Draw(Rectf(floor(j*m_TileWidth), floor((m_MapHeight - (i+1))*m_TileHeight), m_TileWidth, m_TileHeight),
					Rectf((float)tilex, (float)tiley, m_TileWidth, m_TileHeight));
			}
		}
	}
}

void Level::CheckUnwalkable(Rectf& actorShape, Vector2f& actorVelocity)const {
	for (int i{ 0 }; i < m_MapHeight; ++i) {
		for (int j{ 0 }; j < m_MapWidth; ++j) {
			if (m_VectorForeground[((m_MapWidth)*i) + j] != -1) {
				if (utils::IsOverlapping(Rectf(j*m_TileWidth, (m_MapHeight - (i + 1))*m_TileHeight, m_TileWidth, m_TileHeight), actorShape)) {
					if (actorVelocity.x > 0) {//rechts
						actorShape.left = j*m_TileWidth - actorShape.width - 0.1f;

					}
					else if (actorVelocity.x < 0) {//links
						actorShape.left = j*m_TileWidth + m_TileWidth + 0.1f;
					}
					else if (actorVelocity.y > 0) {//Up
						actorShape.bottom = (m_MapHeight - (i + 1))*m_TileHeight - actorShape.height - 0.1f;
					}
					else if (actorVelocity.y < 0) {//Down
						actorShape.bottom = (m_MapHeight - (i + 1))*m_TileHeight + m_TileHeight + 0.1f;
					}
					actorVelocity.x = 0;
					actorVelocity.y = 0;
					return;
				}
			}
		}
	}
}

void Level::CheckGrass(Rectf& actorShape, Vector2f& actorVelocity, int walking, Battle& battle, std::vector<std::shared_ptr<TeamPokemon>>& team)const {
	if (!m_VectorGrass.empty()) {
		for (int i{ 0 }; i < m_MapHeight; ++i) {
			for (int j{ 0 }; j < m_MapWidth; ++j) {
				if (m_VectorGrass[((m_MapWidth)*i) + j] != -1) {
					if (walking != 0) {
						if (utils::IsOverlapping(Rectf(j*m_TileWidth, (m_MapHeight - (i + 1))*m_TileHeight, m_TileWidth, m_TileHeight), actorShape)) {

							if (rand() % 100 < 15) {
								int gemLevel{ 0 };
								for (size_t i{ 0 }; i < team.size(); ++i) {
									gemLevel += team[i]->GetLevel();
								}
								gemLevel = gemLevel / team.size();
								std::shared_ptr<TeamPokemon> enemyTeam{ std::make_shared<TeamPokemon>(PokemonManager::GetInstance().GetPokemon(rand() % 146),rand() % gemLevel + (gemLevel/2)) };
								battle.StartBattle(team, enemyTeam);
							}

							return;
						}
					}
				}
			}
		}
	}
}

void Level::CheckLedge(Rectf& actorShape, Vector2f& actorVelocity, bool& ledgeFound, float& distanceEndLegde, float& toBeginLegde)const {
	if (!m_VectorLedge.empty()) {
		for (int i{ 0 }; i < m_MapHeight; ++i) {
			for (int j{ 0 }; j < m_MapWidth; ++j) {
				if (m_VectorLedge[((m_MapWidth)*i) + j] != -1) {
					if (utils::IsOverlapping(Rectf(j*m_TileWidth, (m_MapHeight - (i + 1))*m_TileHeight, m_TileWidth, m_TileHeight), actorShape)) {
						if (actorVelocity.x > 0) {//rechts
							actorShape.left = j*m_TileWidth - actorShape.width - 0.1f;

						}
						else if (actorVelocity.x < 0) {//links
							actorShape.left = j*m_TileWidth + m_TileWidth + 0.1f;
						}
						else if (actorVelocity.y > 0) {//Up
							actorShape.bottom = (m_MapHeight - (i + 1))*m_TileHeight - actorShape.height - 0.1f; 
						}
						else if (actorVelocity.y < 0) {//Down
							ledgeFound = true;
							distanceEndLegde = actorShape.bottom - ((m_MapHeight - (i + 1))*m_TileHeight - actorShape.height + 0.1f);
							toBeginLegde = (m_MapHeight - (i + 1))*m_TileHeight - actorShape.height;
						}
						actorVelocity.x = 0;
						actorVelocity.y = 0;
						return;
					}
				}
			}
		}
	}
}

void Level::CheckHouse(Rectf& actorShape, Vector2f& actorVelocity)const {
	if (!m_VectorHouses.empty()) {
		for (int i{ 0 }; i < m_MapHeight; ++i) {
			for (int j{ 0 }; j < m_MapWidth; ++j) {
				if (m_VectorHouses[((m_MapWidth)*i) + j] != -1) {
					if (utils::IsOverlapping(Rectf(j*m_TileWidth, (m_MapHeight - (i + 1))*m_TileHeight, m_TileWidth, m_TileHeight), actorShape)) {
						if (actorVelocity.x > 0) {//rechts
							actorShape.left = j*m_TileWidth - actorShape.width - 0.1f;

						}
						else if (actorVelocity.x < 0) {//links
							actorShape.left = j*m_TileWidth + m_TileWidth + 0.1f;
						}
						else if (actorVelocity.y > 0) {//Up
							actorShape.bottom = (m_MapHeight - (i + 1))*m_TileHeight - actorShape.height - 0.1f;
						}
						else if (actorVelocity.y < 0) {//Down
							actorShape.bottom = (m_MapHeight - (i + 1))*m_TileHeight + m_TileHeight + 0.1f;
						}
						actorVelocity.x = 0;
						actorVelocity.y = 0;
						return;
					}
				}
			}
		}
	}
}
