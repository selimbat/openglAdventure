#include "Mesh.h"

Mesh::Mesh()
{
	_VAOId = 0;
	_VBOId = 0;
	_IBOId = 0;
	_indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int vertexCount, unsigned int indexCount)
{
	_indexCount = indexCount;

	glGenVertexArrays(1, &_VAOId);
	glBindVertexArray(_VAOId);

	glGenBuffers(1, &_IBOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBOId);
	unsigned int sizeOfIndexArray = sizeof(indices[0]) * indexCount;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArray, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &_VBOId);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOId);
	unsigned int sizeOfVertexArray = sizeof(vertices[0]) * vertexCount;
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertexArray, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(_VAOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBOId);
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (_IBOId != 0)
	{
		glDeleteBuffers(1, &_IBOId);
		_IBOId = 0;
	}
	if (_VBOId != 0)
	{
		glDeleteBuffers(1, &_VBOId);
		_VBOId = 0;
	}
	if (_VAOId != 0)
	{
		glDeleteVertexArrays(1, &_VAOId);
		_VAOId= 0;
	}
	_indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}
