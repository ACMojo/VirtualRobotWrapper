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

bool VirtualRobotManipulabilityUnmanaged::Init(int argc, char** argv, std::string filename, std::string robotNodeSetName, std::string baseName, std::string tcpName)
{
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

	std::cout << " Using robot at " << filename << std::endl;

	// load robot
	try
	{
		robot = RobotIO::loadRobot(filename);
	}
	catch (VirtualRobotException& e)
	{
		std::cout << " ERROR while creating robot: " << e.what() << std::endl;
		return false;
	}

	if (!robot)
	{
		std::cout << " ERROR while creating robot -> not found" << std::endl;
		return false;
	}

	float manipulabilityAtPose = 0.f;

	try
	{
		tcp = robot->getRobotNode(tcpName);

		std::cout << " TCP: " << tcp->getName() << std::endl;

		baseNode = robot->getRobotNode(baseName);

		std::cout << " RootNode: " << baseNode->getName() << std::endl;

		rns = robot->getRobotNodeSet(robotNodeSetName);

		std::cout << " RootNodeSet: " << rns->getName() << std::endl;
	}
	catch (const std::exception &e) 
	{ 
		std::cout << " ERROR while setting up robot: " << e.what() << std::endl;
		return false;
	}
	catch (...)
	{
		std::cout << " ERROR while  setting up robot" << std::endl;
		return false;
	}

	std::cout << " --- DONE --- " << std::endl;

	return true;
}

void VirtualRobotManipulabilityUnmanaged::GetManipulability(float discrTr, float discrRot, int loops)
{
	std::cout << " --- START --- " << std::endl;

	if (!robot || !rns || !baseNode || !tcp)
	{
		std::cout << " No robot/robot data found " << std::endl;
		return;
	}

	float minB[6];
	float maxB[6];
	float maxManip;

	// automatically determine parameters
	ManipulabilityPtr manipulabilityTest(new Manipulability(robot));
	manipulabilityTest->checkForParameters(rns, 10000, minB, maxB, maxManip, baseNode, tcp);

	minB[3] = 0.f;
	minB[4] = 0.f;
	minB[5] = 0.f;

	maxB[3] = 6.28f;
	maxB[4] = 6.28f;
	maxB[5] = 6.28f;

	ManipulabilityPtr manipulability;
	manipulability.reset(new Manipulability(robot));

	manipulability->initialize(rns, discrTr, discrRot, minB, maxB, VirtualRobot::SceneObjectSetPtr(), VirtualRobot::SceneObjectSetPtr(), baseNode, tcp);

	manipulability->setMaxManipulability(maxManip);

	manipulability->addRandomTCPPoses(loops, 1, false);

	manipulability->print();

	//manipulability->fillHoles();

	WorkspaceDataPtr data = manipulability->getData();

	for (unsigned int x = 0; x < data->getSize(0); x++)
	{
		for (unsigned int y = 0; y < data->getSize(1); y++)
		{
			for (unsigned int z = 0; z < data->getSize(2); z++)
			{
				for (unsigned int a = 0; a < data->getSize(3); a++)
				{
					for (unsigned int b = 0; b < data->getSize(4); b++)
					{
						for (unsigned int c = 0; c < data->getSize(5); c++)
						{
							if (data->hasEntry(x, y, z))
							{
								unsigned char e = data->get(x, y, z, a, b, c);
								float ef = (float)e / 255.0f;

								if (ef < -0.000001 && ef > 0.000001)
									std::cout << "(" << x << "," << y << "," << z << "," << a << "," << b << "," << c << "): " << ef * maxManip << std::endl;
							}
						}
					}
				}
			}
		}
	}

	std::cout << " --- DONE --- " << std::endl;
}

#pragma managed(pop)