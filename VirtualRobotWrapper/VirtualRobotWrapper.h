#pragma once

#include "VirtualRobotWrapperUnmanaged.h"

using namespace System;

namespace VirtualRobotWrapper {
	public ref struct ManipulabilityVoxel {
		unsigned int x;
		unsigned int y;
		unsigned int z;
		unsigned int a;
		unsigned int b;
		unsigned int c;
		float value;
	};

	public ref class VirtualRobotManipulability
	{
	public:
		VirtualRobotManipulability();

		~VirtualRobotManipulability();

		!VirtualRobotManipulability();

		bool Init(int argc, array<String^>^ argv, String^ file, String^ robotNodeSetName, String^ baseName, String^ tcpName);

		array<ManipulabilityVoxel^>^ GetManipulability(float discrTr, float discrRot, int loops, bool fillHoles, bool manipulabilityAsMinMaxRatio);

		array<ManipulabilityVoxel^>^ GetManipulability(float discrTr, float discrRot, int loops, bool fillHoles, bool manipulabilityAsMinMaxRatio, bool penalizeJointLimits, float jointLimitPenalizationFactor);

		array<float>^ MinBox;
		array<float>^ MaxBox;
		float MaxManipulability;

	private:
		VirtualRobotWrapper::Unmanaged::VirtualRobotManipulabilityUnmanaged* unmanagedWrapper;
	}; 
}
