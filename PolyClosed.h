#pragma once
#include "Vec2.h"
#include "Colors.h"
#include "D3DGraphics.h"
#include "dxflib\dl_creationAdapter.h"
#include "dxflib\dl_dxf.h"
#include "Mat3.h"
#include "Drawble.h"
#include "CollidableCircle.h"
#include <vector> 
#include <memory>

class PolyClosed
{
private:
	class Loader : public DL_CreationAdapter
	{

	public:

		Loader(std::string filename)
		{
			//std::unique_ptr ponteiro inteligente que exclui o objeto quando este não sera mais utilizado
			//basicamente chama o destrutor para liberar o objeto da memória depois de usado
			//std::unique_ptr <Dl_Dxf> pDxf(new Dl_Dxf);
			//std::make_unique<T> substitui o new  T () e as vezes fica mais rapido
			//auto funciona como ao var do c#
			auto  pDxf = std::make_unique<DL_Dxf>();
			pDxf->in(filename, this);
		}

		virtual void addVertex(const DL_VertexData& data) override
		{
			//adiciona novo item ao final da lista
			vertices.push_back({ (float)data.x,-(float)data.y });
		}

		operator std::vector<Vec2>&&()
		{
			return std::move(vertices);
		}
	private:
		std::vector<Vec2> vertices;

	};

public:
	class Drawble : public :: Drawble //cria uma extensao interna da classe Drawble
	{
	public:
		Drawble(const PolyClosed& parent)
			:
			parent(parent){}

		void Rasterize(class D3DGraphics& gfx) const override
		{
			//.begin() primeiro item do vector | .end() uma posição depois da ultima posicao do vetor
			for (auto i = parent.vertices.begin(), end = parent.vertices.end() - 1; i != end;i++)
			{
				//i é um iterator que representa um ponteiro que aponta para o objeto na posicao "i"
				//i* aponta para o objeto na posicao i
				gfx.DrawLineClip(trans * *i, trans * *(i + 1), parent.color,clip);
			}
			//.back pega a ultima posicao do vetor | .front() pega a primeira
			gfx.DrawLineClip(trans * parent.vertices.back(), trans * parent.vertices.front(), parent.color, clip);
		}

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
		vertices (Loader(filename)),
		color( color)
		{}
	Drawble GetDrawble() const
	{
		return Drawble(*this);
	}
	inline void PolyClosed::HandleCollision(CollidableCircle& obj)
	{
		//broad phase test
		const auto objAABB = obj.GetAABB();
		const Vec2 center = obj.GetCenter();
		const float radius = obj.GetRadius();
		auto prev = vertices.back();
		for (auto i = vertices.begin(), end = vertices.end(); i<end; i++)
		{
			const auto cur = *i;
			const RectF lineAABB(prev, cur);
			if (obj.GetAABB().Overlaps(lineAABB))
			{
				const auto vecVel = obj.GetVel();
				const auto vecNormal = (cur - prev).CCW90().Normalize();

				//caso  (vecVel * lineNormal) > 0 o objeto já está saindo e não precisa fazer correção de posição.
				if (vecVel * vecNormal < 0.0f)
				{
					const std::vector<Vec2> points = Vec2().CalculateIntersecctionPoints(center, cur, prev, radius);

					if (points.size() == 2)
					{
						const bool contains0 = lineAABB.Contains(points[0]);
						const bool contains1 = lineAABB.Contains(points[1]);

						//colisão com quina EDGE
						if(contains0 != contains1 && !lineAABB.Contains(points[0].MidpointWith(points[1])))
						{
							const Vec2 d1 = center - prev;
							const Vec2 d2 = center - cur;

							const float d1sq = d1.LenSq();
							const float d2sq = d2.LenSq();
							float dsqClosest = d1sq <= d2sq ? d1sq : d2sq;
							Vec2 dClosest = d1sq <= d2sq ? d1 : d2;
							

							if (dClosest * vecVel < 0.0f)
							{
								obj.Rebound(dClosest / sqrt(dsqClosest));
							}
						}
						else if (contains0 || contains1)
						{
							//collision response
							obj.Rebound(vecNormal);
						}

					}
				}
			}
			prev = cur;
		}
	}
private:
	D3DCOLOR color;
	std::vector<Vec2> vertices;
	
};

  