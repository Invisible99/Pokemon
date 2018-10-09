#include "stdafx.h"
#include "AudioManager.h"


AudioManager & AudioManager::GetInstance()
{
	static AudioManager* pSingleton = new AudioManager();
	return *pSingleton;
}

void AudioManager::PlayOverworld()
{
	m_StreamOverworld.Play(true);
}

void AudioManager::PlayBattle()
{
	m_StreamBattle.Play(true);
}

void AudioManager::PlayEvolving()
{
	m_StreamEvolving.Play(true);
}

int AudioManager::GetVolume()
{
	return m_StreamOverworld.GetVolume();
}

void AudioManager::SetVolume(int volume)
{
	m_StreamOverworld.SetVolume(volume);
}

AudioManager::AudioManager()
	: m_StreamOverworld{ "Resources/Audio/RouteMusic.mp3" }
	, m_StreamBattle{ "Resources/Audio/BattleMusic.mp3" }
	, m_StreamEvolving{ "Resources/Audio/Evolution.mp3" }
{
}

