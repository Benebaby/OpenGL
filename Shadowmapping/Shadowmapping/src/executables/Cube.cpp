#include "Cube.h"

Cube::Cube()
{
	m_indices = 36;
	m_points = 24;
	m_size = 1.0f;
	create(1.0f);
}

Cube::Cube(float size)
{
	m_points = 24;
	m_indices = 36;
	m_size = size;
	create(m_size);
}

Cube::~Cube()
{
}

void Cube::create(float size)
{
	GLfloat vertices[] = {
		-1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f
	};

	GLfloat normals[] = {
		 0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,   -1.0f,  0.0f,  0.0f,   -1.0f,  0.0f,  0.0f,   -1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,    0.0f, -1.0f,  0.0f,    0.0f, -1.0f,  0.0f,    0.0f, -1.0f,  0.0f,
		 0.0f,  0.0f, -1.0f,    0.0f,  0.0f, -1.0f,    0.0f,  0.0f, -1.0f,    0.0f,  0.0f, -1.0f
	};

	GLfloat texCoords[] = {
		0.0f,  1.0f,    0.0f,  0.0f,    1.0f,  0.0f,    1.0f,  1.0f,
		0.0f,  1.0f,    0.0f,  0.0f,    1.0f,  0.0f,    1.0f,  1.0f,
		0.0f,  1.0f,    0.0f,  0.0f,    1.0f,  0.0f,    1.0f,  1.0f,
		0.0f,  1.0f,    0.0f,  0.0f,    1.0f,  0.0f,    1.0f,  1.0f,
		0.0f,  1.0f,    0.0f,  0.0f,    1.0f,  0.0f,    1.0f,  1.0f,
		0.0f,  1.0f,    0.0f,  0.0f,    1.0f,  0.0f,    1.0f,  1.0f
	};

	m_points = 24;
	m_indices = 36;

	for (int i = 0; i < m_points; i++)
	{
		m_vertices.push_back(glm::vec4(vertices[i * 3] * size, vertices[i * 3 + 1] * size, vertices[i * 3 + 2] * size, 1.0f));
		m_normals.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
		m_uvs.push_back(glm::vec2(texCoords[i * 2], texCoords[i * 2 + 1]));
	}

	for (int i = 0; i < 6; i++)
	{
		m_index.push_back(i * 4 + 0);
		m_index.push_back(i * 4 + 1);
		m_index.push_back(i * 4 + 2);
		m_index.push_back(i * 4 + 2);
		m_index.push_back(i * 4 + 3);
		m_index.push_back(i * 4 + 0);
	}

	glGenBuffers(1, &m_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec4), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);


	glGenBuffers(1, &m_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_points * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);

	computeTangents();

	glGenBuffers(1, &m_tangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_tangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * sizeof(glm::vec3), &m_tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_indexlist);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices * sizeof(unsigned int), &m_index[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_tangentbuffer);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexlist);

	glBindVertexArray(0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

void Cube::computeTangents()
{
	m_tangents.resize(m_vertices.size());

	for (int i = 0; i < m_indices; i += 3)
	{
		int index_v0 = m_index[i];
		int index_v1 = m_index[i + 1];
		int index_v2 = m_index[i + 2];

		glm::vec4 deltaPos1 = m_vertices[index_v1] - m_vertices[index_v0];
		glm::vec4 deltaPos2 = m_vertices[index_v2] - m_vertices[index_v0];
		/*
		glm::vec4 deltaPos1 = m_vertices[index_v0]-m_vertices[index_v1];
		glm::vec4 deltaPos2 = m_vertices[index_v2]-m_vertices[index_v1];
		*/
		// UV delta

		glm::vec2 deltaUV1 = m_uvs[index_v1] - m_uvs[index_v0];
		glm::vec2 deltaUV2 = m_uvs[index_v2] - m_uvs[index_v0];
		/*
		glm::vec2 deltaUV1 = m_uvs[index_v0]-m_uvs[index_v1];
		glm::vec2 deltaUV2 = m_uvs[index_v2]-m_uvs[index_v1];
		*/
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec4 tmp = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 tangent = glm::vec3(tmp.x, tmp.y, tmp.z);

		// Set the same tangent for all three vertices of the triangle
		m_tangents[index_v0] = tangent;
		m_tangents[index_v1] = tangent;
		m_tangents[index_v2] = tangent;
	}
}

void Cube::render() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, 0);
}
