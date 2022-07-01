#pragma once

class VertexArray
{
public:
	VertexArray(const float* verticies, const int  numVerticies, const int* indicies, const int  numInd);
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

