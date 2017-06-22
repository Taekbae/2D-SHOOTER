/************************************************************************/
/* ������ : �¹���														*/
/* E-mail : lovests2@naver.com											*/
/* ��α� : http://funnism.com											*/
/* ��������                                                              */
/* - 2012�� 4�� 15�� : �̸������ϵ� ��� ��� �� ������� ����             */
/************************************************************************/

#pragma once

typedef map<string, FMOD::Sound*> SOUNDMAP;

enum SOUND {
	BEEP_IN = 0, BEEP_OUT, SUPERBULLET, DAMAGE
};

class SoundManager
{
public:
	~SoundManager(void);
	static SoundManager* Instance() { return &singleton; }

private:
	SoundManager(void);
	static SoundManager singleton;

	//�������

	FMOD::System*	system;
	FMOD::Sound*	music;
	FMOD::Channel*	mchannel;
	FMOD::Channel*	schannel;
	SOUNDMAP		soundMap;

	FMOD::Sound* g_sounds[4] = { 0, }; // ȿ����
	FMOD::Channel* g_channels[4] = { 0, }; // ȿ����

public:
	void InitSound();	// �ʱ� ���������� ��� �ε��Ѵ�.
	void Update();
	bool CreateMusic(char* _filename);
	bool CreateSound(char* _filename);
	void PlaySound(char* _filename); // ����� ���
	void StopSound();

	void Play(int _type); // ȿ���� ���
	FMOD_RESULT r;

	// ����ó�� �ζ��� �Լ�
	inline void ErrorCheck(FMOD_RESULT _result) {
		if (_result != FMOD_OK) {
			MessageBox(NULL, TEXT("FMOD ����"), TEXT("FMOD SOUNDSYSTEM �����Դϴ�. �����ڵ带 Ȯ���ϼ���"), MB_OK);
			exit(-1);
		}
	}

};

#define SOUNDMANAGER() SoundManager::Instance()