#pragma once
#include "SoundStream.h"
class AudioManager
{
public:
	static AudioManager& GetInstance();

	void PlayOverworld();
	void PlayBattle();
	void PlayEvolving();

	int GetVolume();
	void SetVolume(int volume);
private:
	AudioManager();

	SoundStream m_StreamOverworld;
	SoundStream m_StreamBattle;
	SoundStream m_StreamEvolving;
};

