/************************************************************************/
/* 제작자 : 태발이														*/
/* E-mail : lovests2@naver.com											*/
/* 블로그 : http://funnism.com											*/
/* 수정내용                                                              */
/* - 2012년 4월 15일 : 미리컴파일된 헤더 사용 및 헤더파일 정리             */
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

	//멤버변수

	FMOD::System*	system;
	FMOD::Sound*	music;
	FMOD::Channel*	mchannel;
	FMOD::Channel*	schannel;
	SOUNDMAP		soundMap;

	FMOD::Sound* g_sounds[4] = { 0, }; // 효과음
	FMOD::Channel* g_channels[4] = { 0, }; // 효과음

public:
	void InitSound();	// 초기 사운드파일을 모두 로드한다.
	void Update();
	bool CreateMusic(char* _filename);
	bool CreateSound(char* _filename);
	void PlaySound(char* _filename); // 배경음 재생
	void StopSound();

	void Play(int _type); // 효과음 재생
	FMOD_RESULT r;

	// 에러처리 인라인 함수
	inline void ErrorCheck(FMOD_RESULT _result) {
		if (_result != FMOD_OK) {
			MessageBox(NULL, TEXT("FMOD 에러"), TEXT("FMOD SOUNDSYSTEM 에러입니다. 에러코드를 확인하세요"), MB_OK);
			exit(-1);
		}
	}

};

#define SOUNDMANAGER() SoundManager::Instance()