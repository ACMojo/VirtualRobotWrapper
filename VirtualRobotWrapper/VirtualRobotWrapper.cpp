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

array<ManipulabilityVoxel^>^ VirtualRobotWrapper::VirtualRobotManipulability::GetManipulability(float discrTr, float discrRot, int loops)
{
	std::vector<Unmanaged::Data> allResults = unmanagedWrapper->GetManipulability(discrTr, discrRot, loops);

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
