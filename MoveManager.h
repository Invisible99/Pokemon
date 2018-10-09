#pragma once
#include "Move.h"

class MoveManager
{
public:	
	static MoveManager & GetInstance();

	void LoadMoves();

	std::shared_ptr<Move> GetMove(int id);

private:
	MoveManager();
	std::string m_Filename;

	std::vector< std::shared_ptr<Move>> m_pMoves;
};

