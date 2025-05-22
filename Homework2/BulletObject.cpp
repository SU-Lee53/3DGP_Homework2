#include "stdafx.h"
#include "BulletObject.h"
#include "TankPlayer.h"
#include "TankObject.h"
#include "WallsObject.h"
#include "ObstacleObject.h"

void BulletObject::BeginCollision(std::shared_ptr<GameObject> pOther)
{
	if (auto p = dynamic_pointer_cast<TankPlayer>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<TankObject>(pOther)) {
		Reset();
	}
	else if (auto p = dynamic_pointer_cast<BulletObject>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<WallsObject>(pOther)) {

	}
	else if (auto p = dynamic_pointer_cast<ObstacleObject>(pOther)) {

	}
}
