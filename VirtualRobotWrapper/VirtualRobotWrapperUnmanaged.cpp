#pragma managed(push, off)

#include <VirtualRobot/Robot.h>
#include <VirtualRobot/VirtualRobotException.h>
#include <VirtualRobot/Nodes/RobotNode.h>
#include <VirtualRobot/XML/RobotIO.h>
#include <VirtualRobot/RuntimeEnvironment.h>
#include <VirtualRobot/Workspace/Manipulability.h>
#include <VirtualRobot/Workspace/Reachability.h>
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

	float manipulabilityAtPose = 0.f;

	try
	{
		VirtualRobot::RobotNodePtr tcp = robot->getRobotNode("TCP L");

		std::cout << " TCP: " << tcp->getName() << std::endl;

		VirtualRobot::RobotNodePtr rootNode = robot->getRobotNode("Armar3_Base");

		std::cout << " RootNode: " << rootNode->getName() << std::endl;

		VirtualRobot::RobotNodeSetPtr rns = robot->getRobotNodeSet("TorsoLeftArm");

		std::cout << " RootNodeSet: " << rns->getName() << std::endl;

		// CHECK ROBOT WORKSPACE
		Eigen::Matrix4f tcpPose;
		Eigen::Matrix4f gp;

		// reset rob
		gp = Eigen::Matrix4f::Identity();
		robot->setGlobalPose(gp);

		std::cout << " Pose set..." << std::endl;

		// CREATE REACHABILITY DATA
		static float discrTr = 100.0f;
		static float discrRot = 0.5f;
		static float discrTr3 = discrTr*sqrt(3);
		static float discrRot3 = discrRot*sqrt(3);
		float minBounds[6] = { 0,0,0,0,0,0 };
		float maxBounds[6] = { 10000.0f,10000.0f,10000.0f,2 * M_PI,2 * M_PI,2 * M_PI };
		VirtualRobot::ReachabilityPtr reach(new VirtualRobot::Reachability(robot));

		std::cout << " Reachability created..." << std::endl;


		reach->setOrientationType(VirtualRobot::WorkspaceRepresentation::RPY);

		std::cout << " OrientationType set..." << std::endl;

		reach->initialize(rns, discrTr, discrRot, minBounds, maxBounds, VirtualRobot::SceneObjectSetPtr(), VirtualRobot::SceneObjectSetPtr(), rootNode, tcp);

		std::cout << " Reachability created..." << std::endl;

		reach->addRandomTCPPoses(1000, 8, false);

		std::cout << " 1000 random poses created..." << std::endl;

		reach->save("D:\\Temp\\reachability.bin");

		std::cout << " File saved..." << std::endl;

		reach->print();

		float minB[6];
		float maxB[6];
		float maxManip;
		//// automatically determine parameters

		ManipulabilityPtr manipulability(new Manipulability(robot));

		manipulability->checkForParameters(rns, 2000, minB, maxB, maxManip, rootNode, tcp);
		manipulability->initialize(rns, discrTr, discrRot, minBounds, maxBounds, VirtualRobot::SceneObjectSetPtr(), VirtualRobot::SceneObjectSetPtr(), rootNode, tcp);
		manipulability->setMaxManipulability(maxManip);
		//manipulability->addRandomTCPPoses(1000, 8, false);

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

		manipulability->save("D:\\Temp\\manipulability.bin");
		manipulability->print();
	}
	catch (const std::exception &e) 
	{ 
		std::cout << " ERROR while creating Manipulability: " << e.what() << std::endl;
		return false;
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