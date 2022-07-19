#pragma once
#include <vector>

class VertexArray
{
public:
	VertexArray(const std::vector<struct Vertex>& verticies, const std::vector<unsigned int >& indicies);
	~VertexArray();

	void Bind();
	int GetNumIndices()const { return _numInd; }
private:
	unsigned int va;
	unsigned int vb;
	unsigned int ib;
	int _numVert;
	int _numInd;
};

