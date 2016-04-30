#pragma once
#include "PolyClosed.h"
#include "Camera.h"


class Ship :public CollidableCircle
{

public:
	class Drawble : public ::Drawble //cria uma extensao interna da classe Drawble
	{

	public:
		Drawble(const Ship& parent)
			:
			parent(parent)
		{
			Transform(Mat3::Translation(parent.pos) * Mat3::Rotation(parent.angle));
		}

		void Rasterize(D3DGraphics& gfx) const override
		{
			PolyClosed::Drawble d = parent.model.GetDrawble();
			d.Transform(trans);
			gfx.Draw(d);


			const Vec2 shieldCenter = trans * Vec2(0.0f, 0.0f);
			gfx.DrawCircle(shieldCenter, parent.shieldRadius, parent.shieldColor);
		}
		
	private:
		const Ship& parent;
	};


public:
	Ship(std::string filename,Vec2 pos = {0.0f,0.0f})
		:
		pos(pos),
		model(filename)
	{}
	Drawble GetDrawble() const
	{
		return Drawble( *this);
	}
	void Update(float dt)//delta time
	{
		//angular (1st order then 0th order)
		if(angAccelDir == 0.0f)
		{
			if (abs(angVel) <= angAccel*dt)
				angVel = 0.0f;
			else
				angVel -= copysign(angAccel, angVel) * dt;
		}

		angVel += angAccel * angAccelDir *dt;
		angle += angVel  * dt;
		//reseta angulo para que ele fique entre 0 e 360º
		angle = fmodf(angle, 2.0f*PI);


		//linear (1st order then 0th order)
		vel += Vec2({0.0f,-1.0f}).Rotation(angle) * accel* thrust * dt;
		pos += vel*dt;
	}
	void FocusOn(Camera& cam ) const
	{
		cam.MoveTo(pos);
	}
	//control functions
	void Thrust()
	{
		thrust = 1.0f;
	}
	void StopThrusting()
	{
		thrust = 0.0f;
	}
	void Spin(float dir)
	{
		angAccelDir = copysign(1.0f, dir);
	}
	void StopSpining(float dir)
	{
		if (angAccelDir == copysign(1.0f, dir))
		{
			angAccelDir = 0.0f;
		}
	}

private:
	// strutural
	PolyClosed model;
	const int shieldRadius = 42;
	const D3DCOLOR shieldColor = D3DCOLOR_XRGB(0,255,0);

	//linear
	Vec2 pos;
	Vec2 vel = {0.0f,0.0f};//utilizado para velocidade linear
	const float accel = 0.2f * 60.0f * 60.0f;//aceleracao
	float thrust = 0.0f;

	//angulas
	float angle = 0.0f;//0.0f;
	float angVel = 0.0f;//velocidade angular
	const float angAccel = 0.002f  * 60.0f * 60.0f;//velocidade angular 
	float angAccelDir = 0.0f;//derecao da acelaracao // +1 = rotacao na direção contraria ao relógio,0 = não está mudando 

};