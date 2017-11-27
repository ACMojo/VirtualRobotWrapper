// Dies ist die Haupt-DLL.
#pragma once

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <msclr\marshal_cppstd.h>

#include "VirtualRobotWrapper.h"
#include "VirtualRobotWrapperUnmanaged.h"

using namespace VirtualRobotWrapper;

VirtualRobotManipulability::VirtualRobotManipulability() 
{
	unmanagedWrapper = new Unmanaged::VirtualRobotManipulabilityUnmanaged();

	MinBox = gcnew array<float>(6);
	MaxBox = gcnew array<float>(6);
	MaxManipulability = 1;

	std::cout << " --- CREATED --- " << std::endl;
}

VirtualRobotManipulability::~VirtualRobotManipulability()
{
	this->VirtualRobotManipulability::!VirtualRobotManipulability();
}

VirtualRobotManipulability::!VirtualRobotManipulability()
{ 
	delete unmanagedWrapper; 
}

bool VirtualRobotManipulability::Init(int argc, array<String^>^ argv, String^ file, String^ robotNodeSetName, String^ baseName, String^ tcpName)
{
	std::string filename = msclr::interop::marshal_as<std::string>(file);
	std::string robotNodeSetNameU = msclr::interop::marshal_as<std::string>(robotNodeSetName);
	std::string baseNameU = msclr::interop::marshal_as<std::string>(baseName);
	std::string tcpNameU = msclr::interop::marshal_as<std::string>(tcpName);

	return unmanagedWrapper->Init(argc, NULL, filename, robotNodeSetNameU, baseNameU, tcpNameU);
}

array<ManipulabilityVoxel^>^ VirtualRobotWrapper::VirtualRobotManipulability::GetManipulability(float discrTr, float discrRot, int loops, bool fillHoles, bool manipulabilityAsMinMaxRatio)
{
	return VirtualRobotWrapper::VirtualRobotManipulability::GetManipulability(discrTr, discrRot, loops, fillHoles, manipulabilityAsMinMaxRatio, false, 50.0);
}

array<ManipulabilityVoxel^>^ VirtualRobotWrapper::VirtualRobotManipulability::GetManipulability(float discrTr, float discrRot, int loops, bool fillHoles, bool manipulabilityAsMinMaxRatio, bool penalizeJointLimits, float jointLimitPenalizationFactor)
{
	std::vector<Unmanaged::Data> allResults = unmanagedWrapper->GetManipulability(discrTr, discrRot, loops, fillHoles, manipulabilityAsMinMaxRatio, penalizeJointLimits, jointLimitPenalizationFactor);
	
	for (int i = 0; i < 6; i++)
		MinBox[i] = unmanagedWrapper->minB[i];

	for (int i = 0; i < 6; i++)
		MaxBox[i] = unmanagedWrapper->maxB[i];

	MaxManipulability = unmanagedWrapper->maxManip;

	array<ManipulabilityVoxel^>^ wrappedResult = gcnew array<ManipulabilityVoxel^>(allResults.size());

	for (int i = 0; i < allResults.size(); i++) 
	{
		ManipulabilityVoxel^ voxel = gcnew ManipulabilityVoxel();

		Unmanaged::Data result = allResults[i];

		voxel->x = result.x;
		voxel->y = result.y;
		voxel->z = result.z;
		voxel->a = result.a;
		voxel->b = result.b;
		voxel->c = result.c;
		voxel->value = result.value;

		wrappedResult[i] = voxel;
	}

	return wrappedResult;
}
