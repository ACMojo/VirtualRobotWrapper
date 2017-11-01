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
		std::cout << " ERROR while creating robot 1" << std::endl;
		std::cout << e.what();
		return false;
	}

	if (!robot)
	{
		std::cout << " ERROR while creating robot 2" << std::endl;
		return false;
	}

	float manipulabilityAtPose = 0.f;

	try
	{
		//VirtualRobot::RobotNodePtr tcp = robot->getRobotNode("TCP L");
		VirtualRobot::RobotNodePtr tcp = robot->getRobotNode("tcp");

		std::cout << " TCP: " << tcp->getName() << std::endl;

		VirtualRobot::RobotNodePtr rootNode = robot->getRobotNode("root");

		std::cout << " RootNode: " << rootNode->getName() << std::endl;

		VirtualRobot::RobotNodeSetPtr rns = robot->getRobotNodeSet("robotNodeSet");

		std::cout << " RootNodeSet: " << rns->getName() << std::endl;

		// CHECK ROBOT WORKSPACE
		Eigen::Matrix4f tcpPose;
		Eigen::Matrix4f gp;

		// reset rob
		gp = robot->getGlobalPose();

		std::cout << " Pose set..." << std::endl;

		// CREATE REACHABILITY DATA
		static float discrTr = 50.f;
		static float discrRot = 0.2f;

		//float minBounds[6];
		//float maxBounds[6];

		//VirtualRobot::ReachabilityPtr reachTest(new VirtualRobot::Reachability(robot));

		//reachTest->checkForParameters(rns, 1000, minBounds, maxBounds, rootNode, tcp);

		//VirtualRobot::ReachabilityPtr reach;
		//reach.reset(new VirtualRobot::Reachability(robot));

		//std::cout << " Reachability created..." << std::endl;
		//
		//reach->setOrientationType(VirtualRobot::WorkspaceRepresentation::Hopf);

		//std::cout << " OrientationType set..." << std::endl;

		////reach->initialize(rns, discrTr, discrRot, minBounds, maxBounds, VirtualRobot::SceneObjectSetPtr(), VirtualRobot::SceneObjectSetPtr(), rootNode, tcp);

		//std::cout << " Reachability created..." << std::endl;

		////reach->addRandomTCPPoses(10000, 8, true);

		//std::cout << " 1000 random poses created..." << std::endl;

		////reach->save("C:\\Users\\alexa\\Desktop\\reachability.bin");

		//std::cout << " File saved..." << std::endl;

		//reach->print();

		float minB[6];
		float maxB[6];
		float maxManip;
		//// automatically determine parameters

		ManipulabilityPtr manipulabilityTest(new Manipulability(robot));
		manipulabilityTest->checkForParameters(rns, 10000, minB, maxB, maxManip, rootNode, tcp);
	
		minB[3] = 0.f;
		minB[4] = 0.f;
		minB[5] = 0.f;

		maxB[3] = 6.28f;
		maxB[4] = 6.28f;
		maxB[5] = 6.28f;

		ManipulabilityPtr manipulability;
		manipulability.reset(new Manipulability(robot));
		manipulability->initialize(rns, discrTr, discrRot, minB, maxB, VirtualRobot::SceneObjectSetPtr(), VirtualRobot::SceneObjectSetPtr(), rootNode, tcp);
		manipulability->setMaxManipulability(maxManip);

		manipulability->addRandomTCPPoses(10000, 1, false);

		manipulability->print();

		//manipulability->save("C:\\Users\\alexa\\Desktop\\manipulability.bin");

		try
		{
			float x[6];

			x[0] = -149.f;
			x[1] = 864.f;
			x[2] = 20.f;
			for (int i = 0; i < 360; i++)
			{
				x[3] = (float)i;
				for (int j = 0; j < 360; i++)
				{
					x[4] = (float)j;
					for (int k = 0; k < 360; i++)
					{
						x[5] = (float)k;


						manipulability->vector2Matrix(x, gp);

						manipulabilityAtPose = manipulability->getManipulabilityAtPose(gp);

						if (manipulabilityAtPose > 0.000001f || manipulabilityAtPose < -0.0000001f)
						{
							std::cout << "ManipulabilityAtPose: " << manipulabilityAtPose << std::endl;
						}
					}
				}

			}

			
		}
		catch (...)
		{
			std::cout << " ERROR while checking ManipulabilityAtPose" << std::endl;
			return false;
		}
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