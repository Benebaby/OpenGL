#include "Sphere.h"

Sphere::Sphere()
{
	m_radius = 1.0f;
	m_resolution = 20;
	m_Center = glm::vec3(0);
	create(m_Center, m_radius, m_resolution);
}

Sphere::Sphere( float radius)
{
	m_radius = radius;
	m_resolution = 20;
	m_Center = glm::vec3(0);
	create( m_Center,  m_radius, m_resolution);
}

Sphere::Sphere( glm::vec3 center, float radius)
{
	m_radius = radius;
	m_resolution = 20;
	m_Center = center;
	create(m_Center, m_radius, m_resolution);
}

Sphere::Sphere( float radius, int resolution)
{
	m_radius = radius;
	m_resolution = resolution;
	m_Center = glm::vec3(0);
	create(m_Center, m_radius, m_resolution);
}

Sphere::Sphere( glm::vec3 center, float radius, int resolution)
{
	m_radius = radius;
	m_resolution = resolution;
	m_Center = center;
	create( m_Center,  m_radius, m_resolution);
}

Sphere::~Sphere()
{
}

glm::vec3 *Sphere::get_center()
{
	return &m_Center;
}

float Sphere::get_radius()
{
	return m_radius;
}

void Sphere::create(  glm::vec3 center, float radius, int resolution)
{
	// iniatialize the variable we are going to use
	float u, v;
	float phi, theta;
	float x, y, z;
	int offset = 0, i, j;

	// create points
	for ( j = 0; j <= resolution; j++)  //theta
		for ( i = 0; i <= resolution; i++) //phi
		{
			u = i /(float)resolution;		phi   = 2* glm::pi<float>() * u;
			v = j /(float)resolution;		theta =    glm::pi<float>() * v;

			x = m_Center.x + radius * sin(theta) * cos(phi);
			y = m_Center.y + radius * sin(theta) * sin(phi);
			z = m_Center.z + radius * cos(theta);

			m_vertices.push_back(glm::vec4( x, y, z, 1.0f));
			m_normals.push_back(glm::vec3( x - m_Center.x, y - m_Center.y, z - m_Center.z) / radius);
			m_uvs.push_back(glm::vec2( u, 1-v));
		}
	m_points = m_vertices.size();

	// create index list
	for ( j = 0; j < resolution; j++)
	{
		for ( i = 0; i < resolution; i++)
		{
			// 1. Triangle
			m_index.push_back( offset + i );
			m_index.push_back( offset + i + resolution+1);
			m_index.push_back( offset + i + resolution+1 + 1);

			// 2. Triangle
			m_index.push_back( offset + i + resolution+1 + 1);
			m_index.push_back( offset + i + 1);
			m_index.push_back( offset + i );
		}
		offset += resolution+1;
	}
	m_indices = m_index.size();

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

void Sphere::computeTangents()
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

void Sphere::render() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, 0);
}
