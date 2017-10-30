#pragma once

#include <string>

namespace VirtualRobotWrapper
{
	namespace Unmanaged
	{
		public class VirtualRobotManipulabilityUnmanaged
		{
		public:
			VirtualRobotManipulabilityUnmanaged();

			~VirtualRobotManipulabilityUnmanaged();

			bool Run(std::string filename);
		};
	}
}