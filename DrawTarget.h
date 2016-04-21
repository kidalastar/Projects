#pragma once
#include "Drawble.h"

class DrawTarget
{
public:
	virtual void Draw(Drawble& obj) = 0;
};
