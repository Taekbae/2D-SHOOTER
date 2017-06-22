/************************************************************************/
/* 제작자 : 태발이														*/
/* E-mail : lovests2@naver.com											*/
/* 블로그 : http://funnism.com											*/
/* 수정내용                                                              */
/* - 2012년 4월 15일 : 미리컴파일된 헤더 사용 및 헤더파일 정리             */
/************************************************************************/

#include "stdafx.h"

SoundManager SoundManager::singleton;

SoundManager::SoundManager(void)
{
	ErrorCheck(FMOD::System_Create(&system));

	ErrorCheck(system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));

	ErrorCheck(system->setSoftwareChannels(32));

	ErrorCheck(system->setHardwareChannels(32));

	ErrorCheck(system->init(32, FMOD_INIT_NORMAL, 0));
}

SoundManager::~SoundManager(void)
{
	SOUNDMAP::iterator iter;
	FMOD::Sound* sound;
	for (iter = soundMap.begin(); iter != soundMap.end(); ++iter) {
		sound = iter->second;
		sound->release();
	}
	music->release();

	system->release();
}

void SoundManager::InitSound()
{
	// CreateSound("meow.mp3");

	r = System_Create(&system);
	r = system->init(320, FMOD_INIT_NORMAL, 0);
	ErrorCheck(r);
	r = system->createSound("Beep_in.mp3", FMOD_DEFAULT, NULL, &g_sounds[BEEP_IN]);
	ErrorCheck(r);
	r = system->createSound("Beep_out.mp3", FMOD_DEFAULT, NULL, &g_sounds[BEEP_OUT]);
	ErrorCheck(r);
	r = system->createSound("Superbullet.mp3", FMOD_DEFAULT, NULL, &g_sounds[SUPERBULLET]);
	ErrorCheck(r);
	r = system->createSound("Damage.mp3", FMOD_DEFAULT, NULL, &g_sounds[DAMAGE]);
	ErrorCheck(r);
}

void SoundManager::Update()
{
	system->update();
}

bool SoundManager::CreateMusic(char* _filename)
{
	FMOD_RESULT result;
	string soundname = _filename;

	music->release();
	result = system->createStream(soundname.c_str(), FMOD_LOOP_NORMAL, 0, &music);
	if (result != FMOD_OK) {
		return false;
	}
	return true;
}

bool SoundManager::CreateSound(char* _filename)
{
	FMOD_RESULT result;
	string SoundName = _filename;

	FMOD::Sound* sound = soundMap[SoundName];

	if (!sound) {
		result = system->createSound(SoundName.c_str(), FMOD_LOOP_NORMAL, 0, &sound);
		if (result != FMOD_OK) {
			return false;
		}

		soundMap[SoundName] = sound;
		if (result != FMOD_OK) {
			return false;
		}

		return true;
	}

	return false;
}

void SoundManager::PlaySound(char* _filename)
{
	string soundname = _filename;
	schannel->setVolume(0.2);

	FMOD::Sound* sound = soundMap[soundname];

	if (!sound) {
		CreateSound(const_cast<char*>(soundname.c_str()));
		sound = soundMap[soundname];
	}
	bool playing;
	schannel->isPlaying(&playing);
	if (!playing) {
		ErrorCheck(system->playSound(FMOD_CHANNEL_FREE, sound, false, &schannel));
	}
}

void SoundManager::StopSound()
{
	schannel->stop();
}

void SoundManager::Play(int _type)
{
	system->update();
	r = system->playSound(FMOD_CHANNEL_FREE, g_sounds[_type], false, &g_channels[_type]);
	ErrorCheck(r);
}