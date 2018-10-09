#include "stdafx.h"
#include "Split.h"
#include "MoveManager.h"

#include <fstream>
#include <iostream>
#include <algorithm>


MoveManager::MoveManager()
{
	m_Filename = "Resources/Text/Moves.csv";
}


MoveManager& MoveManager::GetInstance() {
	static MoveManager* pSingleton = new MoveManager();
	return *pSingleton;
}

void MoveManager::LoadMoves() {
	std::ifstream moveFile{ m_Filename };

	if (!moveFile) {
		std::cout << "Openen van bestand mislukt: " << m_Filename << "\n";
		std::cin.get();
		return;
	}

	std::string moveStr{};
	while (moveFile && std::getline(moveFile, moveStr))
	{
		std::vector<std::string> moveVec = explodeString(moveStr, ',');
		bool tm;
		if (moveVec[8] == "TRUE") {
			tm = true;
		}
		else {
			tm = false;
		}
			m_pMoves.push_back(std::make_shared<Move>( moveVec[0], PokemonType::Types(std::stoi(moveVec[1])), Move::AttackCategory(std::stoi(moveVec[2])), Move::AttackEffect(std::stoi(moveVec[3])), StatusEffects::AttackStatus(std::stoi(moveVec[4])),
			std::stoi(moveVec[5]), std::stof(moveVec[6]), std::stof(moveVec[7]), tm ));
	}
}

std::shared_ptr<Move> MoveManager::GetMove(int id) {
	return m_pMoves[id];
}