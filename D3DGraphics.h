/****************************************************************************************** 
 *	Chili DirectX Framework Version 14.03.22											  *	
 *	D3DGraphics.h																		  *
 *	Copyright 2014 PlanetChili <http://www.planetchili.net>								  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include <d3d9.h>
#include "Vec2.h"
#include "Colors.h"
#include <functional>
#include "DrawTarget.h"

class D3DGraphics : public DrawTarget
{
public:
	D3DGraphics( HWND hWnd );
	~D3DGraphics();
	void PutPixel( int x,int y,D3DCOLOR c );
	D3DCOLOR GetPixel( int x,int y ) const;
	inline void DrawLine( Vec2& pt1,Vec2& pt2,D3DCOLOR c )
	{
		DrawLine( (int)pt1.x,(int)pt1.y,(int)pt2.x,(int)pt2.y,c );
	}
	void DrawLine( int x1,int y1,int x2,int y2,D3DCOLOR c );
	void DrawCircle( int centerX,int centerY,int radius,D3DCOLOR c );
	void BeginFrame();
	void EndFrame();

	virtual void Draw(Drawble&  obj)  override
	{
		obj.Rasterize(*this);
	}

	// Compute the bit code for a point (x, y) using the clip rectangle
	// bounded diagonally by (xmin, ymin), and (xmax, ymax)

	// Cohen–Sutherland clipping algorithm clips a line from
	// P0 = (p0.x, p0.y) to P1 = (p1.x, p1.y) against a rectangle with 
	// diagonal from (xmin, ymin) to (xmax, ymax).
	void DrawLineClip(Vec2 p0,Vec2 p1,D3DCOLOR color,const RectF& clip)
	{
		enum OutCode
		{
			INSIDE = 0, // 0000
			LEFT = 1,   // 0001
			RIGHT = 2,  // 0010
			BOTTOM = 4, // 0100
			TOP = 8,    // 1000
		};


		std::function<OutCode(float , float )> ComputeOutCode = [&clip](float x, float y) -> OutCode
		{
			OutCode code = INSIDE;   // initialised as being inside of clip window
			if (x < clip.left)           // to the left of clip window
				code = (OutCode)LEFT;
			else if (x > clip.right)  // to the right of clip window
				code = (OutCode)RIGHT;
			if (y < clip.top)           // below the clip window
				code = (OutCode)BOTTOM;
			else if (y > clip.bottom)      // above the clip window
				code = (OutCode)TOP;
			return code;
		};

		// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
		OutCode outcode0 = ComputeOutCode(p0.x, p0.y);
		OutCode outcode1 = ComputeOutCode(p1.x, p1.y);

		bool accept = false;

		while (true) {
			if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
				accept = true;
				break;
			}
			else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
				break;
			}
			else {
				// failed both tests, so calculate the line segment to clip
				// from an outside point to an intersection with clip edge
				float x, y;

				// At least one endpoint is outside the clip rectangle; pick it.
				OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

				// Now find the intersection point;
				// use formulas y = p0.y + slope * (x - p0.x), x = p0.x + (1 / slope) * (y - p0.y)
				if (outcodeOut & TOP) {           // point is above the clip rectangle
					x = p0.x + (p1.x - p0.x) * (clip.bottom - p0.y) / (p1.y - p0.y);
					y = clip.bottom;
				}
				else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
					x = p0.x + (p1.x - p0.x) * (clip.top - p0.y) / (p1.y - p0.y);
					y = clip.top;
				}
				else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
					y = p0.y + (p1.y - p0.y) * (clip.right - p0.x) / (p1.x - p0.x);
					x = clip.right;
				}
				else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
					y = p0.y + (p1.y - p0.y) * (clip.left - p0.x) / (p1.x - p0.x);
					x = clip.left;
				}

				// Now we move outside point to intersection point to clip
				// and get ready for next pass.
				if (outcodeOut == outcode0) {
					p0.x = x;
					p0.y = y;
					outcode0 = ComputeOutCode(p0.x, p0.y);
				}
				else {
					p1.x = x;
					p1.y = y;
					outcode1 = ComputeOutCode(p1.x, p1.y);
				}
			}
		}
		if (accept) 
		{
			// Following functions are left for implementation by user based on
			// their platform (OpenGL/graphics.h etc.)
			DrawLine(p0, p1, color);
			//DrawLine(Vec2{(float)( p0.x + 0.5f),(float)(p0.y + 0.5f)}, (Vec2{(float)(p1.x + 0.5f),(float)(p1.y + 0.5f) }), color);
		}
	}

public:
	static const unsigned int	SCREENWIDTH =	800;
	static const unsigned int	SCREENHEIGHT =	600;
private:
	const D3DCOLOR		FILLVALUE =		BLACK;
	IDirect3D9*			pDirect3D;
	IDirect3DDevice9*	pDevice;
	IDirect3DSurface9*	pBackBuffer;
	D3DCOLOR*			pSysBuffer;
};