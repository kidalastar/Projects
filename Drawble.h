#pragma once
#include "Mat3.h"
#include "Rect.h"

class Drawble
{
	
public:
	Drawble()
		:
		trans(Mat3::Identity()),
		clip( -INT_MAX, INT_MAX, -INT_MAX, INT_MAX)
	{}
	void Transform(const Mat3& mat)
	{
		trans = mat * trans;
	} 
	void Clip(const RectI& rect)
	{
		clip.ClipTo(rect);
	}
	virtual void Rasterize(class D3DGraphics& gfx) = 0;
protected:
	Mat3 trans;
	RectI clip;//cliping utilizando alg de clipping
};