#pragma once
#include "Vec2.h"
#include "Colors.h"
#include "D3DGraphics.h"
#include "dxflib\dl_creationAdapter.h"
#include "dxflib\dl_dxf.h"
#include "Mat3.h"
#include "Drawble.h"
#include <vector> 
#include <memory>


class PolyCloseLoader : public DL_CreationAdapter
{
public:

	PolyCloseLoader(std::string filename)
	{
		//std::unique_ptr ponteiro inteligente que exclui o objeto quando este não sera mais utilizado
		//basicamente chama o destrutor para liberar o objeto da memória depois de usado
		//std::unique_ptr <Dl_Dxf> pDxf(new Dl_Dxf);
		//std::make_unique<T> substitui o new  T () e as vezes fica mais rapido
		//auto funciona como ao var do c#
		auto  pDxf = std::make_unique<DL_Dxf>();
		pDxf->in(filename, this);
	}

	virtual void addVertex( const DL_VertexData& data) override
	{
		//adiciona novo item ao final da lista
		vertices.push_back({(float)data.x,-(float)data.y});
	}
    
	std::vector<Vec2>& GetVertices()
	{
		return vertices;
	}
private:
	std::vector<Vec2> vertices;

};



class PolyClosed
{

public:
	class Drawble : public :: Drawble //cria uma extensao interna da classe Drawble
	{
	public:
		Drawble(const PolyClosed& parent)
			:
			parent(parent){}

		virtual void Rasterize(class D3DGraphics& gfx) override
		{
			//.begin() primeiro item do vector | .end() uma posição depois da ultima posicao do vetor
			for (auto i = parent.vertices.begin(), end = parent.vertices.end() - 1; i != end;i++)
			{
				//i é um iterator que representa um ponteiro que aponta para o objeto na posicao "i"
				//i* aponta para o objeto na posicao i
				gfx.DrawLineClip(trans * *i, trans * *(i + 1), parent.color);
			}
			//.back pega a ultima posicao do vetor | .front() pega a primeira
			gfx.DrawLineClip(trans * parent.vertices.back(), trans * parent.vertices.front(), parent.color);
		}

		//virtual void Rasterize(class D3DGraphics& gfx) override
		//	{
		//		//.begin() primeiro item do vector | .end() uma posição depois da ultima posicao do vetor
		//		for (auto i = parent.vertices.begin(), end = parent.vertices.end() - 1; i != end;i++)
		//		{
		//			//i é um iterator que representa um ponteiro que aponta para o objeto na posicao "i"
		//			//i* aponta para o objeto na posicao i
		//			gfx.DrawLineClip(i->Rotation(0.1f),  (i + 1)->Rotation(0.1f), parent.color);
		//		}
		//		//.back pega a ultima posicao do vetor | .front() pega a primeira
		//		gfx.DrawLineClip(parent.vertices.back().Rotation(0.1f),parent.vertices.front().Rotation(0.1f), parent.color);
		//	}
	private:
		const PolyClosed& parent;
	};

public:
    //constructor simplificado, c++ 11
	PolyClosed(std::initializer_list<Vec2> vList,D3DCOLOR color = WHITE )
	   :     
		vertices( std::move(vList) ),//move evita a copia de objetos que serão destruidos depois. procurar por move semantics
		color( color)
		{}
	PolyClosed(std::string filename,D3DCOLOR color = WHITE )
		:
		vertices (PolyCloseLoader(filename).GetVertices()),
		color( color)
		{}
	Drawble GetDrawble() const
	{
		return Drawble(*this);
	}
private:
	D3DCOLOR color;
	std::vector<Vec2> vertices;
	
};

  