/*
Copyright 2019 Meta Berghauser Pont

This file is part of PST.

PST is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version. The GNU General Public License
is intended to guarantee your freedom to share and change all versions
of a program--to make sure it remains free software for all its users.

PST is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PST. If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include <cmath>
#include "Vec2.h"

#define REAL float

typedef float2 VEC2;
typedef VEC2 COORDS;

#define PI 3.1415926535897932384626433832795

template <typename T> 
inline T angleDiff(T a1, T a2) 
{
	using namespace std;
	T a = abs(a1 - a2);
	return (a > 180) ? 360 - a : a;
}

template <typename T>
inline T reverseAngle(T a) {
	return a < 180 ? (a + 180) : (a - 180);
}

template <typename T, typename T2>
inline T lerp(T a, T b, T2 t) {
	return (T)(a + (b - a) * t);
}

template <typename T>
inline T sqr(T a) {
	return a * a;
}

template <class T>
inline bool same_sign(T a, T b)
{
	return a * b >= 0;
}

template <typename T> 
inline T log(T b, T v)
{
	return log(v) / log(b);
}

template <typename TVec>
inline auto OrientationAngleFromVector(const TVec& v) -> decltype(v.x)
{
	typedef decltype(v.x) real_t;
	auto angle = (atan2(v.y, v.x) * (real_t)180) / (real_t)PI;
	if (angle < 0)
		angle += (real_t)360;
	return angle;
}

/**
* Degrees to Syntax Angle (see Hillier and Iida)
* [0..180] -> [0..2]
*/
// TODO: Find a better place for this formula?
template <typename T>
inline float SyntaxAngleWeightFromDegrees(T degrees)
{
	return (1.0f / 90.0f) * (float)degrees;
}



///////////////////////////////////////////////////////////////////////////////
//
//  Line
//

template <typename TVec>
class TLine {
public:
	TVec p1;
	TVec p2;

	TLine() {}
	TLine(const TVec& _p1, const TVec& _p2) : p1(_p1), p2(_p2) {}

	inline auto getLength() -> decltype(p1.x) const { return (p1 - p2).getLength(); }
	inline bool operator==(const TLine& rhs) const { return (p1 == rhs.p1) && (p2 == rhs.p2); }
};

class CLine2f : public TLine < float2 >
{
public:
	CLine2f() {}
	CLine2f(const float2& _p1, const float2& _p2) : TLine(_p1, _p2) {}
};

class CLine2d : public TLine < double2 >
{
public:
	CLine2d() {}
	CLine2d(const double2& _p1, const double2& _p2) : TLine(_p1, _p2) {}
	explicit operator CLine2f() const { return CLine2f((float2)p1, (float2)p2); }
};

typedef CLine2f LINE;


template <typename TLine> 
inline bool FindLineIntersection2(const TLine& a, const TLine& b, decltype(((TLine*)nullptr)->p1.x)* pT1, decltype(((TLine*)nullptr)->p1.x)* pT2, const decltype(((TLine*)nullptr)->p1.x) eps = 0.00001)
{

	if ((a.p1 == b.p1) || (a.p1 == b.p2)) {
		*pT1 = 0;
		*pT2 = (a.p1 == b.p1) ? 0.f : 1.f;
		return true;
	}

	if ((a.p2 == b.p2) || (a.p2 == b.p1)) {
		*pT1 = 1;
		*pT2 = (a.p2 == b.p1) ? 0.f : 1.f;
		return true;
	}

	auto va = a.p2 - a.p1;
	auto vb = b.p2 - b.p1;

	const auto d = crp(va, vb);
	if (0 == d)
		return false;

	const auto dInv = 1.0f / d;

	auto v = a.p1 - b.p1;

	const auto t1 = crp(vb, v) * dInv;
	if ((t1 < -eps) || (t1 > eps + 1))
		return false;

	const auto t2 = crp(va, v) * dInv;
	if ((t2 < -eps) || (t2 > eps + 1))
		return false;

	*pT1 = t1;
	*pT2 = t2;

	return true;
}

template <typename TVec2>
inline bool Find2DRayIntersection(const TVec2& p0, const TVec2& v0, const TVec2& p1, const TVec2& v1, decltype(TVec2::x)& t0, decltype(TVec2::x)& t1)
{
	auto d = crp(v0, v1);
	if (0 == d)
		return false;

	const auto dInv = 1.0f / d;

	const auto v = p0 - p1;

	t0 = crp(v1, v) * dInv;
	t1 = crp(v0, v) * dInv;

	return true;
}