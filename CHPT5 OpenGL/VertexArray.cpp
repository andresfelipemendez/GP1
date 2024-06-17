#include "VertexArray.h"
#include "GL/glew.h"

VertexArray::VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices)
	:mNumIndices(numIndices),mNumVerts(numVerts)
{
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);
	glGenBuffers(1, &mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		numVerts * 3 * sizeof(float),
		verts,
		GL_STATIC_DRAW
	);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		mIndexBuffer * sizeof(unsigned int),
		indices,
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 3,
		0
	);


}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}