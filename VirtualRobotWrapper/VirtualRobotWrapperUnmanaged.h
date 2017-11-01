#pragma once

#pragma managed(push, off)
#include <VirtualRobot/Robot.h>
#pragma managed(pop)

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

			bool Init(int argc, char** argv, std::string filename, std::string robotNodeSetName, std::string baseName, std::string tcpName);

			void GetManipulability(float discrTr, float discrRot, int loops);

		private:
			VirtualRobot::RobotPtr robot = NULL;
			VirtualRobot::RobotNodeSetPtr rns = NULL;
			VirtualRobot::RobotNodePtr baseNode = NULL;
			VirtualRobot::RobotNodePtr tcp = NULL;
		};
	}
}