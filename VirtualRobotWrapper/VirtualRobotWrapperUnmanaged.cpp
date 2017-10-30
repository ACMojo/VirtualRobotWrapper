#pragma managed(push, off)

#include <VirtualRobot/Robot.h>
#include <VirtualRobot/VirtualRobotException.h>
#include <VirtualRobot/Nodes/RobotNode.h>
#include <VirtualRobot/XML/RobotIO.h>
#include <VirtualRobot/RuntimeEnvironment.h>
#include <VirtualRobot/Workspace/Manipulability.h>
#include <VirtualRobot/IK/PoseQualityExtendedManipulability.h>

#include <iostream>
#include <string>

#include "VirtualRobotWrapperUnmanaged.h"

using namespace VirtualRobot;
using namespace VirtualRobotWrapper::Unmanaged;

VirtualRobotManipulabilityUnmanaged::VirtualRobotManipulabilityUnmanaged()
{
}


VirtualRobotManipulabilityUnmanaged::~VirtualRobotManipulabilityUnmanaged()
{
}

bool VirtualRobotManipulabilityUnmanaged::Run(std::string filename)
{
	int argc = 0;
	char** argv = NULL;

	std::cout << " --- START --- " << std::endl;

	VirtualRobot::init(argc, argv, "VirtualRobotManipulability");

	VirtualRobot::RuntimeEnvironment::considerKey("robot");
	VirtualRobot::RuntimeEnvironment::processCommandLine(argc, argv);
	VirtualRobot::RuntimeEnvironment::print();

	if (VirtualRobot::RuntimeEnvironment::hasValue("robot"))
	{
		std::string robFile = VirtualRobot::RuntimeEnvironment::getValue("robot");

		if (VirtualRobot::RuntimeEnvironment::getDataFileAbsolute(robFile))
		{
			filename = robFile;
		}
	}

	std::cout << "Using robot at " << filename << std::endl;

	// load robot
	RobotPtr robot;

	try
	{
		robot = RobotIO::loadRobot(filename);
	}
	catch (VirtualRobotException& e)
	{
		std::cout << " ERROR while creating robot" << std::endl;
		std::cout << e.what();
		return false;
	}

	if (!robot)
	{
		std::cout << " ERROR while creating robot" << std::endl;
		return false;
	}

	float manipulabilityAtPose;

	try
	{
		ManipulabilityPtr manipulability(new Manipulability(robot));
		//float minB[6];
		//float maxB[6];
		//float maxManip;
		//// automatically determine parameters
		//manipulability->checkForParameters(rns, 2000, minB, maxB, maxManip, baseNode, tcpNode);
		//manipulability->initialize(rns, discrTr, discrRo, minB, maxB, staticModel, dynamicModel, baseNode, tcpNode);
		//manipulability->setMaxManipulability(maxManip);
		manipulability->print();

		try
		{
			Eigen::Matrix4f pose = Eigen::Matrix4f::Identity();

			manipulabilityAtPose = manipulability->getManipulabilityAtPose(pose);
		}
		catch (...)
		{
			std::cout << " ERROR while checking ManipulabilityAtPose" << std::endl;
			return false;
		}
	}
	catch (...)
	{
		std::cout << " ERROR while creating Manipulability" << std::endl;
		return false;
	}

	std::cout << "ManipulabilityAtPose: " << manipulabilityAtPose << std::endl;

	return true;
}

#pragma managed(pop)