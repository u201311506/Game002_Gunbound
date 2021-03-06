#include "GLogicCamera.h"
#include "GBaseActor.h"
#include "GameApp.h"
#include "GEngine.h"
#include "GWorld.h"

GLogicCamera::GLogicCamera(GEngine* engine) :
	mX(0), mY(0), 
	mWidth(GameApp::WIDTH), mHeight(GameApp::HEIGHT),
	mHWidth(GameApp::WIDTH * 0.5f), mHHeight(GameApp::HEIGHT * 0.5f)
{
	mEngine = engine;
}

GLogicCamera::~GLogicCamera()
{
}

void GLogicCamera::Initialize() {
	mWorld = mEngine->GetWorld();
}

void GLogicCamera::Update(float dt) {
	if (mCurrentTarget != nullptr) {
		float cx = mCurrentTarget->GetX();
		float cy = mCurrentTarget->GetY();
		float ww = mWorld->GetWidth();
		float limitRightX = ww - mHWidth;


		mX = cx - mHWidth;
		mY = cy - mHHeight;
		//if (cx > mHWidth) { mX = cx - mHWidth; }
		if (cx < mHWidth) { mX = 0; }
		else if (cx > limitRightX) { mX = cx - (mHWidth + (cx - limitRightX)); }

		//if (cy > mHHeight) { mY = cy - mHHeight; }
	}
}