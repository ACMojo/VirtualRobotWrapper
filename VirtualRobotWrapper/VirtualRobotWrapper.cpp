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

void VirtualRobotWrapper::VirtualRobotManipulability::GetManipulability(float discrTr, float discrRot, int loops)
{
	return unmanagedWrapper->GetManipulability(discrTr, discrRot, loops);
}
