#include "GPlayerControl.h"
#include "GunboundGameApp.h"
#include "GPlayer.h"
#include <FrameworkUPC\MathUtils.h>

GPlayerControl::GPlayerControl(GPlayer* player) :
	mIsLeft(false), mIsRight(false),
	mIsUp(false), mIsDown(false),
	mIsSpacePressed(false),
	mMoveSpeed(600),
	mCurrentAngle(-M_PI*0.25),
	mAngleChangeSpeed(M_PI*0.25),
	mMaxStrength(1200),
	mAlreadyShot(false),
	mChargingShot(false)
{
	mPlayer = player;
}

GPlayerControl::~GPlayerControl()
{
}

void GPlayerControl::Initialize(Sprite* playerBody, Sprite* cannonBody) {
	mPlayerBody = playerBody;
	mCannonBody = cannonBody;
	mStrengthChargeBar=GunboundGameApp::GET_GAMEAPP()->GetHud()->GetPlayerChargeBar();
}

void GPlayerControl::OnKeyDown(SDL_Keycode key) {
	switch (key)
	{
	case SDLK_LEFT:
		mIsLeft = true;
		break;
	case SDLK_RIGHT:
		mIsRight = true;
		break;
	case SDLK_UP:
		mIsUp = true;
		break;
	case SDLK_DOWN:
		mIsDown = true;
		break;
	case SDLK_SPACE:
		mIsSpacePressed = true;
		break;
	}
}

void GPlayerControl::OnKeyUp(SDL_Keycode key) {
	switch (key)
	{
	case SDLK_LEFT:
		mIsLeft = false;
		break;
	case SDLK_RIGHT:
		mIsRight = false;
		break;
	case SDLK_UP:
		mIsUp = false;
		break;
	case SDLK_DOWN:
		mIsDown = false;
		break;
	case SDLK_SPACE:
		mIsSpacePressed = false;
		mAlreadyShot = false;
		break;
	}
}

void GPlayerControl::Update(float dt)
{
	if (mIsSpacePressed)
	{
		if (mChargingShot)
		{
			mCurrentCharge += dt;

			if (mCurrentCharge >= 1.0f)
			{
				mCurrentCharge = 1.0f;
				Shoot();
			}

			mStrengthChargeBar->SetScaleX(mCurrentCharge);
		}
		else if (!mAlreadyShot)
		{
			ChargeShot();
		}
	}
	else
	{
		if (mChargingShot) Shoot();
	}

	float currentSpeed = 0;
	if (mIsLeft) currentSpeed -= mMoveSpeed;
	if (mIsRight) currentSpeed += mMoveSpeed;
	mPlayer->Translate(currentSpeed*dt, 0);

	if (mIsUp) mCurrentAngle -= mAngleChangeSpeed*dt;
	if (mIsDown) mCurrentAngle += mAngleChangeSpeed*dt;
	mCurrentAngle = MathUtils::Clamp(mCurrentAngle, -M_PI*0.5f, 0);

	Vector2 pos = mPlayerBody->GetPosition();
	mCannonBody->SetX(pos.x + mPlayerBody->GetVisibleWidth()*0.6f);
	mCannonBody->SetY(pos.y - 6);
	mCannonBody->SetRotationZ(mCurrentAngle + M_PI*0.25f);
}

void GPlayerControl::ChargeShot()
{
	if (!mChargingShot)
	{
		mChargingShot = true;
		mCurrentCharge = 0.0f;
		mStrengthChargeBar->SetScaleX(mCurrentCharge);
	}
}

void GPlayerControl::Shoot()
{
	mChargingShot = false;
	if (mIsSpacePressed) mAlreadyShot = true;

	mPlayer->Shoot(mCurrentAngle, mMaxStrength*mCurrentCharge);
}