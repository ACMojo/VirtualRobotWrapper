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
#include <vector>

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

std::vector<Data> VirtualRobotManipulabilityUnmanaged::GetManipulability(float discrTr, float discrRot, int loops, bool fillHoles, bool manipulabilityAsMinMaxRatio, bool penalizeJointLimits, float jointLimitPenalizationFactor)
{
	std::cout << " --- START --- " << std::endl;

	if (!robot || !rns || !baseNode || !tcp)
	{
		std::cout << " No robot/robot data found " << std::endl;
		return std::vector<Data>(0);
	}

	// automatically determine parameters
	ManipulabilityPtr manipulabilityTest(new Manipulability(robot));
	manipulabilityTest->checkForParameters(rns, 1000, minB, maxB, maxManip, baseNode, tcp);

	minB[3] = 0.f;
	minB[4] = 0.f;
	minB[5] = 0.f;

	maxB[3] = 6.28f;
	maxB[4] = 6.28f;
	maxB[5] = 6.28f;

	ManipulabilityPtr manipulability;
	manipulability.reset(new Manipulability(robot));

	PoseQualityManipulability::ManipulabilityIndexType indexType = manipulabilityAsMinMaxRatio ? PoseQualityManipulability::ManipulabilityIndexType::eMinMaxRatio : PoseQualityManipulability::ManipulabilityIndexType::eMultiplySV;

	if (penalizeJointLimits)
	{
		PoseQualityExtendedManipulabilityPtr m(new PoseQualityExtendedManipulability(robot->getRobotNodeSet(rns->getName()), indexType));
	    m->penalizeJointLimits(true, jointLimitPenalizationFactor);
		manipulability->setManipulabilityMeasure(m);
	}
	else 
	{
		PoseQualityManipulabilityPtr m(new PoseQualityManipulability(robot->getRobotNodeSet(rns->getName()), indexType));
		manipulability->setManipulabilityMeasure(m);
	}

	manipulability->initialize(rns, discrTr, discrRot, minB, maxB, VirtualRobot::SceneObjectSetPtr(), VirtualRobot::SceneObjectSetPtr(), baseNode, tcp);

	manipulability->setMaxManipulability(maxManip);

	manipulability->addRandomTCPPoses(loops, 1, false);

	if (fillHoles)
		manipulability->fillHoles();

	WorkspaceDataPtr data = manipulability->getData();

	unsigned int sizeX = data->getSize(0);
	unsigned int sizeY = data->getSize(1);
	unsigned int sizeZ = data->getSize(2);
	unsigned int sizeA = data->getSize(3);
	unsigned int sizeB = data->getSize(4);
	unsigned int sizeC = data->getSize(5);

	std::vector<Data> allResults;
	allResults.reserve(data->getVoxelFilledCount());

	for (unsigned int x = 0; x < sizeX; x++)
	{
		for (unsigned int y = 0; y < sizeY; y++)
		{
			for (unsigned int z = 0; z < sizeZ; z++)
			{
				for (unsigned int a = 0; a < sizeA; a++)
				{
					for (unsigned int b = 0; b < sizeB; b++)
					{
						for (unsigned int c = 0; c < sizeC; c++)
						{
							if (data->hasEntry(x, y, z))
							{
								unsigned char e = data->get(x, y, z, a, b, c);
								float ef = (float)e / 255.0f;

								if (e != 0)
								{
									Data result;
									result.x = x;
									result.y = y;
									result.z = z;
									result.a = a;
									result.b = b;
									result.c = c;
									result.value = ef * maxManip;

									allResults.push_back(result);
								}
							}
						}
					}
				}
			}
		}
	}

	std::cout << " --- DONE --- " << std::endl;

	return allResults;
}

#pragma managed(pop)