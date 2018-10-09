#pragma once
#include "Move.h"
class KnowMove
{
public:
	explicit KnowMove(std::shared_ptr<Move> move);

	void SetPP(int newPP);

	int GetPP();
	std::shared_ptr<Move> GetMove();

private:
	std::shared_ptr<Move> m_pMove;

	int m_PP;
};

