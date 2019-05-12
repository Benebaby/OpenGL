#include "Cube.h"

Cube::Cube()
{
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(3, VertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_v[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_n[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_t[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices * sizeof(GL_UNSIGNED_INT), &_index[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Cube::~Cube()
{
}

void Cube::render() 
{
	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_INT, 0);
}
