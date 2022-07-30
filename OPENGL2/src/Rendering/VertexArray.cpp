#include "VertexArray.h"

#include <vector>
#include "../Entity/Mesh.h"
#include "Debuger.h"
struct Vertex;

VertexArray::VertexArray(const std::vector<Vertex>& verticies, const std::vector<unsigned int >& indicies)
{
	_numInd = indicies.size();
	_numVert = verticies.size();
	// Create vertexArray object
	GLCall(glGenVertexArrays(1, &va));
	GLCall(glBindVertexArray(va));
	// set up vertex buffer
	GLCall(glGenBuffers(1, &vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verticies.size(), &verticies[0], GL_STATIC_DRAW));

	// set up index buffer
	GLCall(glGenBuffers(1, &ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* indicies.size(), &indicies[0], GL_STATIC_DRAW));
	// layout of vertex buffer
	
	GLCall(glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)0))
	GLCall(glEnableVertexAttribArray(0))
	
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex,Normal)))
	GLCall(glEnableVertexAttribArray(1))

	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)))
	GLCall(glEnableVertexAttribArray(2))

	GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)))
	GLCall(glEnableVertexAttribArray(3))

	GLCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)))
	GLCall(glEnableVertexAttribArray(4))

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
