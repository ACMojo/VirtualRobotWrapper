#pragma once

#include "VirtualRobotWrapperUnmanaged.h"

using namespace System;

namespace VirtualRobotWrapper {
	public ref class VirtualRobotManipulability
	{
	public:
		VirtualRobotManipulability();

		~VirtualRobotManipulability();

		!VirtualRobotManipulability();

		bool Init(int argc, array<String^>^ argv, String^ file, String^ robotNodeSetName, String^ baseName, String^ tcpName);

		void GetManipulability(float discrTr, float discrRot, int loops);

	private:
		VirtualRobotWrapper::Unmanaged::VirtualRobotManipulabilityUnmanaged* unmanagedWrapper;
	}; 
}
