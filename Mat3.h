#pragma once
#include "Vec2.h"
#include <string.h>

template <typename T>
class _Mat3
{
public:
	 _Mat3& operator =(const _Mat3& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	 _Mat3& operator *=(const T rhs)
	 {
		 for each(T&  e in elements)
		 {
			 e *= rhs; 
		 }
		 return *this;
	 }
	 _Mat3& operator *=(const T rhs) const
	 {
		 _Mat3 result = *this;
		 return result*=rhs;
	 }
	 _Mat3 operator  *(const _Mat3& rhs) const
	 {
		 _Mat3 result;

		 for (int i = 0;i < 3;i++)
		 {
			 for (int j = 0; j < 3; j++)
			 {
				 T sum = (T)0.0;
				 for (int k = 0; k < 3; k++)
				 {
					 sum += elements[i][k] * rhs.elements[k][j];
				 }
				 result.elements[i][j] = sum;
			 }
		 }
		 return result;
	 }
	 _Mat3& operator *=(const _Mat3& rhs) 
	 {
		 return *this = *this * rhs;
	 }
	 _Vec2<T> operator *(const _Vec2<T> rhs ) const
	 {
		 _Vec2<T> result;
		 result.x = elements[0][0] * rhs.x + elements[0][1] * rhs.y + elements[0][2];
		 result.y = elements[1][0] * rhs.x + elements[1][1] * rhs.y + elements[1][2];
		 return result;
	 }
	 static _Mat3 Identity()
	 {
		 _Mat3 t = { (T)1.0,(T)0.0,(T)0.0,(T)0.0,(T)1.0,(T)0.0,(T)0.0,(T)0.0,(T)1.0 };
		 return t;
	 }
	 static _Mat3 Rotation(T theta)
	 {
		 const T cosTheta = cos(theta);
		 const T sinTheta = cos(theta);
		 _Mat3 r = { cosTheta,-sinTheta,(T)0.0,sinTheta,cosTheta,(T)0.0,(T)0.0,(T)0.0,(T)1.0 };
		 return r;
	 }
	 static _Mat3 Scaling(float factor)
	 {
		   _Mat3 s = { factor,(T)0.0,(T)0.0,(T)0.0,factor, (T)0.0,(T)0.0,(T)0.0, (T)1.0 };
		   return s;
	 }
	 static _Mat3 Translation(_Vec2<T> offset)
	 {
		  _Mat3 t = { (T)1.0,(T)0.0,offset.x,(T)0.0,(T)1.0,offset.y,(T)0.0,(T)0.0,(T)1.0 };
		  return t;
	 }
public: 
	T elements[3][3];
};

typedef _Mat3<float> Mat3;
//typedef _Mat3<double> Matd3;