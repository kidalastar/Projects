/****************************************************************************************** 
 *	Chili DirectX Framework Version 14.03.22											  *	
 *	Game.cpp																			  *
 *	Copyright 2014 PlanetChili.net <http://www.planetchili.net>							  *
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
 *************************
 *****************************************************************/
#include "Game.h"
#include "Mat3.h"

Game::Game( HWND hWnd,KeyboardServer& kServer,const MouseServer& mServer) :
	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer ),
	ship("shipd.dxf"),
	port (gfx,{0,D3DGraphics::SCREENHEIGHT-1,0,D3DGraphics::SCREENWIDTH - 1 }),
	cam ( port,(float)port.GetWidth(), (float)port.GetHeight() )
{
}

Game::~Game()
{
}

void Game::Go()
{	
	HandleInput();
	UpdateModel();
	gfx.BeginFrame();
	ComposeFrame();
	gfx.EndFrame();
 }

void Game::HandleInput()
{
	const KeyEvent key = kbd.ReadKey();
	switch (key.GetCode())
	{
	case VK_LEFT:
		if (key.IsPress())
			ship.Spin(-1.0f);
		else
			ship.StopSpining(-1.0f);
		break;
	case VK_RIGHT:
		if (key.IsPress())
			ship.Spin(1.0f);
		else
			ship.StopSpining(1.0f);
		break;
	case VK_SPACE:
		if (key.IsPress())
			ship.Thrust();
		else
			ship.StopThrusting();
		break;
	}
}

void Game::UpdateModel( )
{
	ship.Update();
}

void Game::ComposeFrame()
{
	cam.Draw(ship.GetDrawble());
}
