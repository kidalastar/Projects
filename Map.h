#pragma once
#include "PolyClosed.h"


class Map
{
public:

	Map(std::string filename)
		:
		model(filename)
	{}

	PolyClosed::Drawble GetDrawble() const
	{
		return model.GetDrawble();
	}


private:
	// strutural
	PolyClosed model;

};