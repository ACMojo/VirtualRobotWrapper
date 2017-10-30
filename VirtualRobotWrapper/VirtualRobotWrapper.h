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

		bool Run(String^ file);

	private:
		VirtualRobotWrapper::Unmanaged::VirtualRobotManipulabilityUnmanaged* unmanagedWrapper;
	}; 
}
