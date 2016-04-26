#pragma once
#include "DrawTarget.h"
#include "Vec2.h"


class ViewPort : public DrawTarget
{
public:

	ViewPort(DrawTarget& next,const RectF& rect)
		:
		next(next),
		clip(rect)
	{ }
	virtual void Draw(Drawble& obj) override
	{
		obj.Transform(Mat3::Translation({(float)clip.left,(float)clip.top}));
		obj.Clip(clip);
		next.Draw(obj);
	}
	float GetWidth() const
	{
		return clip.GetWidth();
	}
	float GetHeight() const
	{
		return clip.GetHeight();
	}

private:
	DrawTarget& next;
	RectF clip;
};