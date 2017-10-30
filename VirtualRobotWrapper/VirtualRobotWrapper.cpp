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

bool VirtualRobotManipulability::Run(System::String^ file)
{
	std::string filename = msclr::interop::marshal_as<std::string>(file);

	return unmanagedWrapper->Run(filename);
}