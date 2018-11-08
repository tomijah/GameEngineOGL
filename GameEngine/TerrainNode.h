#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "Utils.h"
#include "Mesh.h"

using namespace std;

class TerrainNode
{
public:
	TerrainNode(TerrainNode* parent);
	~TerrainNode();
private:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<TerrainNode> children;
	GLuint VAO, VBO, EBO;
};

