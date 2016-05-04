#pragma once
#include "ICollidable.h"
class CollidableCircle : public ICollidable
{
public :
	virtual float GetRadius() const = 0;
	virtual Vec2 GetCenter() const = 0;
	virtual void Rebound(Vec2 normal) = 0;
};