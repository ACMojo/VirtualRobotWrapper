#include <MathGeoLib/Geometry/OBB.h>

#include "OBBWrapper.h"

MathGeoLibWrapper::OBBWrapper::OBBWrapper(array<array<double>^>^ points)
{
	vec* vecPoints = new vec[points->Length];
	for (int i = 0; i < points->Length; i++)
	{
		vecPoints[i] = vec(points[i][0], points[i][1], points[i][2]);
	}

	OBB obb = OBB::OptimalEnclosingOBB(vecPoints, points->Length);

	Position = gcnew array<double>(3);
	for (int i = 0; i < 3; i++)
	{
		Position[i] = (double)obb.pos[i];
	}

	HalfExtents = gcnew array<double>(3);
	for (int i = 0; i < 3; i++)
	{
		HalfExtents[i] = (double)obb.r[i];
	}

	Axis = gcnew array<array<double>^>(3);
	for (int i = 0; i < 3; i++)
	{
		Axis[i] = gcnew array<double>(3);
		for (int j = 0; j < 3; j++)
		{
			Axis[i][j] = (double)obb.axis[i][j];
		}
	}
}

MathGeoLibWrapper::OBBWrapper::~OBBWrapper()
{
	this->OBBWrapper::!OBBWrapper();
}

MathGeoLibWrapper::OBBWrapper::!OBBWrapper()
{

}
