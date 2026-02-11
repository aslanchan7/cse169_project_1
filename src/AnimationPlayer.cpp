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
		clip->Evaluate(time, Window::joints);
	}
}

void AnimationPlayer::SetClip(AnimationClip* c) {
	clip = c;
	time = 0.0f;
	channelLastKeyframeIdx.clear();
	channelLastKeyframeIdx.resize(clip->numChannels, 0);
}