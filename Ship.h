#pragma once
#include "PolyClosed.h"


class Ship
{
public:

	Ship(std::string filename,Vec2 pos = {0.0f,0.0f})
		:
		pos(pos),
		model(filename)
	{}

	PolyClosed::Drawble GetDrawble() const
	{
		PolyClosed::Drawble d = model.GetDrawble();
		d.Transform(Mat3::Translation(pos) * Mat3::Rotation(angle));
		return d;
	}
	void Update()
	{
		//angular (1st order then 0th order)
		angVel += angAccel * angAccelDir;
		angle += angVel;
		//reseta angulo para que ele fique entre 0 e 360º
		angle = fmodf(angle, 2.0f*PI);


		//linear (1st order then 0th order)
		vel += Vec2({0.0f,-1.0f}).Rotation(angle) * accel* thrust;
		pos += vel;
	}
	void Thrust()  	{ thrust = 1.0f;}
	void StopThrusting() { thrust = 0.0f; }
	void Spin(float dir) { angAccelDir = dir; }
	void StopSpining(float dir) { angAccelDir == dir ? angAccelDir = 0.0f : angAccelDir; }

private:
	// strutural
	PolyClosed model;

	//linear
	Vec2 pos;
	Vec2 vel = {0.0f,0.0f};//utilizado para velocidade linear
	const float accel = 0.2f;//aceleracao
	float thrust = 0.0f;

	//angulas
	float angle = 0.0f;
	float angVel = 0.0f;//velocidade angular
	const float angAccel = 0.01f;//velocidade angular 
	// +1 = rotacao na direção contraria ao relógio,0 = não está mudando 
	float angAccelDir = 0.0f;//derecao da acelaracao

};