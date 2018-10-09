#pragma once
#include "Battle.h"
class Level
{
public:
	Level();
	void DrawBackground();
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, bool& ledgeFound, float& distanceEndLegde, float& toBeginLegde) const;
	void CheckGrass(Rectf& actorShape, Vector2f& actorVelocity, int walking, Battle& battle, std::vector<std::shared_ptr<TeamPokemon>>& team)const;
	Rectf GetBoundaries();
	bool& IsLedgeFound();
private:
	Texture m_TileMapWorld;
	Texture m_TileMapHouses;

	Rectf m_Boundaries;

	float const m_TileWidth{ 16 };
	float const m_TileHeight{ 16 };
	float const m_TileColumnsWorld{ 28 };
	float const m_TileColumnsHouses{ 64};

	int m_MapWidth{ 0 };
	int m_MapHeight{ 0 };

	bool m_LegdeFound{ false };

	std::string m_FileNameHouses;
	std::string m_FileNameBackground;
	std::string m_FileNameForeground;
	std::string m_FileNameLedge;
	std::string m_FileNameGrass;

	std::vector<int> m_VectorHouses;
	std::vector<int> m_VectorBackground;
	std::vector<int> m_VectorForeground;
	std::vector<int> m_VectorLedge;
	std::vector<int> m_VectorGrass;

	void LoadFile(std::string filename, std::vector<int>& fill, bool count);
	void DrawHouses(std::vector<int>& toDraw)const;
	void DrawMap(std::vector<int>& toDraw)const;
	void CheckUnwalkable(Rectf& actorShape, Vector2f& actorVelocity)const;	
	void CheckLedge(Rectf& actorShape, Vector2f& actorVelocity, bool& ledgeFound, float& distanceEndLegde, float& toBeginLegde)const;
	void CheckHouse(Rectf& actorShape, Vector2f& actorVelocity)const;
};

