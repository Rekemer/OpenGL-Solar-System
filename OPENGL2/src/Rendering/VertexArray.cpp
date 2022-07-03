#include "VertexArray.h"
#include "Debuger.h"
VertexArray::VertexArray(const float* verticies, const int numVerticies, const int* indicies, const int numInd):
_numVert(numVerticies),_numInd(numInd)
{
	// Create vertexArray object

	
	GLCall(glGenVertexArrays(1, &va));
	GLCall(glBindVertexArray(va));
	// set up vertex buffer
	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4*5*sizeof(float), verticies,GL_STATIC_DRAW));

	// set up index buffer
	GLCall(glGenBuffers(1, &ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInd * sizeof(int), indicies, GL_STATIC_DRAW));
	// layout of vertex buffer
	
	GLCall(glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5 *  sizeof(float), (void*)0))
		GLCall(glEnableVertexAttribArray(0))
	
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float))))
	GLCall(glEnableVertexAttribArray(1))


}

VertexArray::~VertexArray()
{
	// Clear data
	GLCall(glDeleteBuffers(1, &va));
	GLCall(glDeleteBuffers(1, &ib));
	GLCall(glDeleteVertexArrays(1, &vb));
}

void VertexArray::Bind()
{
	// Set OPENGL state
	GLCall(glBindVertexArray(va));
}
