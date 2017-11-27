#pragma once

#pragma managed(push, off)
#include <VirtualRobot/Robot.h>
#pragma managed(pop)

#include <string>

namespace VirtualRobotWrapper
{
	namespace Unmanaged
	{
		public struct Data {
			unsigned int x;
			unsigned int y;
			unsigned int z;
			unsigned int a;
			unsigned int b;
			unsigned int c;
			float value;
		};

		public class VirtualRobotManipulabilityUnmanaged
		{
		public:
			VirtualRobotManipulabilityUnmanaged();

			~VirtualRobotManipulabilityUnmanaged();

			bool Init(int argc, char** argv, std::string filename, std::string robotNodeSetName, std::string baseName, std::string tcpName);

			std::vector<Data> GetManipulability(float discrTr, float discrRot, int loops, bool fillHoles, bool manipulabilityAsMinMaxRatio, bool penalizeJointLimits, float jointLimitPenalizationFactor);

			float minB[6];
			float maxB[6];
			float maxManip;

		private:
			VirtualRobot::RobotPtr robot = NULL;
			VirtualRobot::RobotNodeSetPtr rns = NULL;
			VirtualRobot::RobotNodePtr baseNode = NULL;
			VirtualRobot::RobotNodePtr tcp = NULL;
		};
	}
}