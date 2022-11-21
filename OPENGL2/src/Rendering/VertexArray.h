#pragma once
#include <vector>

class VertexArray
{
public:
	VertexArray() = default ;
	void Init(const std::vector<struct Vertex>& verticies, const std::vector<unsigned int >& indicies);
	~VertexArray();

	void Bind();
	int GetNumIndices()const { return _numInd; }
	int GetVertexArrayId()const { return va; }
	int GetVertexBufferId()const { return vb; }
	int GetIndexBufferId()const { return ib; }
private:
	unsigned int va;
	unsigned int vb;
	unsigned int ib;
	int _numVert;
	int _numInd;
};

