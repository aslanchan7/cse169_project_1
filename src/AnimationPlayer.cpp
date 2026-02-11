#include "AnimationPlayer.h"
#include "Window.h"

AnimationPlayer::AnimationPlayer() {
	time = 0.0f;
	clip = nullptr;
}

AnimationPlayer::~AnimationPlayer() {
}

void AnimationPlayer::Update(float deltaTime) {
	if (clip != nullptr) {
		time += deltaTime;
		// Loop animation
		//if (time > clip->timeEnd) {
			//time = fmod(time - clip->timeStart, clip->timeEnd - clip->timeStart) + clip->timeStart;
			//for (int i = 0; i < channelLastKeyframeIdx.size(); i++) {
				//channelLastKeyframeIdx[i] = 0;
			//}
		//}
		clip->Evaluate(time, Window::joints, channelLastKeyframeIdx);
	}
}

void AnimationPlayer::SetClip(AnimationClip* c) {
	clip = c;
	time = 0.0f;
	channelLastKeyframeIdx.clear();
	channelLastKeyframeIdx.resize(clip->numChannels, 0);
}