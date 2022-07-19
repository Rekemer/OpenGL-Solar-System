#include "VertexArray.h"

#include <vector>
#include "../Entity/Mesh.h"
#include "Debuger.h"
struct Vertex;

VertexArray::VertexArray(const std::vector<Vertex>& verticies, const std::vector<unsigned int >& indicies)
{
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

	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)))
	GLCall(glEnableVertexAttribArray(2))



	//	// create buffers/arrays
	//	glGenVertexArrays(1, &va);
	//glGenBuffers(1, &vb);
	//glGenBuffers(1, &ib);

	//glBindVertexArray(va);
	//// load data into vertex buffers
	//glBindBuffer(GL_ARRAY_BUFFER, vb);
	//// A great thing about structs is that their memory layout is sequential for all its items.
	//// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	//// again translates to 3/2 floats which translates to a byte array.
	//glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	//// set the vertex attribute pointers
	//// vertex Positions
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//// vertex normals
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//// vertex texture coords
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	//// vertex tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	//// vertex bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	//// ids
	//glEnableVertexAttribArray(5);
	//glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	//// weights
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
	//glBindVertexArray(0);



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
