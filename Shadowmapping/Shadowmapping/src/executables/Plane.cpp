#include "Plane.h"


Plane::Plane()
{
	create( glm::vec3( -1, 0, -1), glm::vec3( -1, 0, 1), glm::vec3( 1, 0, 1), glm::vec3( 1, 0, -1),
		    glm::vec2( 0, 1), glm::vec2( 0, 0), glm::vec2( 1, 0), glm::vec2( 1, 1));
}

Plane::Plane( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
	create( a, b, c, d, glm::vec2( 0, 1), glm::vec2( 0, 0), glm::vec2( 1, 0), glm::vec2( 1, 1));
}

Plane::Plane( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	create( a, b, c, d, tca, tcb, tcc, tcd);
}

Plane::~Plane()
{
}

void Plane::create( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	m_vertices.push_back( glm::vec4(a, 1.0f));
	m_vertices.push_back( glm::vec4(b, 1.0f));
	m_vertices.push_back( glm::vec4(c, 1.0f));
	m_vertices.push_back( glm::vec4(d, 1.0f));

	glm::vec3 n = glm::vec3(0.0, 1.0, 0.0);

	m_normals.push_back( n);
	m_normals.push_back( n);
	m_normals.push_back( n);
	m_normals.push_back( n);

	m_uvs.push_back( tca);
	m_uvs.push_back( tcb);
	m_uvs.push_back( tcc);
	m_uvs.push_back( tcd);

	m_points = 4;

	m_index.push_back( 0);
	m_index.push_back( 1);
	m_index.push_back( 2);

	m_index.push_back( 2);
	m_index.push_back( 3);
	m_index.push_back( 0);

	m_indices = 6;

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

void Plane::computeTangents()
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

void Plane::render() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, 0);
}

