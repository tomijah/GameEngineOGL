#pragma once
#include "ShaderBase.h"
#include "Utils.h"

class MeshShader:
	public ShaderBase
{
public:
	MeshShader();
	~MeshShader()
	{
		std::cout << "Shader delete" << std::endl;
	}
};
