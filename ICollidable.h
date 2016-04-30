#pragma once
#include "Rect.h"

class ICollidable
{
public:
	virtual RectF GetAABB() const = 0;
	virtual Vec2 GetVel() const = 0;
};