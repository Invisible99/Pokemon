#include "stdafx.h"
#include "KnowMove.h"


KnowMove::KnowMove(std::shared_ptr<Move> move)
{
	m_pMove = move;
	m_PP = m_pMove->GetPPMax();
}

void KnowMove::SetPP(int newPP)
{
	m_PP = newPP;
}

int KnowMove::GetPP()
{
	return m_PP;
}

std::shared_ptr<Move> KnowMove::GetMove()
{
	return m_pMove;
}
