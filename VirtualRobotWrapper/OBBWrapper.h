#pragma once

using namespace System;

namespace MathGeoLibWrapper {
	public ref class OBBWrapper
	{
	public:
		OBBWrapper(array<array<double>^>^ points);

		~OBBWrapper();

		!OBBWrapper();

		array<double>^ Position;
		array<double>^ HalfExtents;
		array<array<double>^>^ Axis;
	};
}

