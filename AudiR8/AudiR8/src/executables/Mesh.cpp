#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::string filename)
{
	readObj(filename);
	size = m_vertices.size();
	glGenVertexArrays(1, &m_vertexArrayHandle);
	glBindVertexArray(m_vertexArrayHandle);

	glGenBuffers(4, m_vertexBufferHandle);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle[1]);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (m_texCoords.size() >= m_vertices.size())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle[2]);
		glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//calcuate tangents from vertices and texcoords
		for (unsigned int i = 0; i < m_vertices.size(); i += 3)
		{
			glm::vec3 deltaPos1 = m_vertices[i] - m_vertices[i + 1];
			glm::vec3 deltaPos2 = m_vertices[i + 2] - m_vertices[i + 1];

			glm::vec2 deltaUV1 = m_texCoords[i] - m_texCoords[i + 1];
			glm::vec2 deltaUV2 = m_texCoords[i + 2] - m_texCoords[i + 1];

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tmp = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			glm::vec3 tangent = glm::vec3(tmp.x, tmp.y, tmp.z);

			m_tangents.push_back(tangent);
			m_tangents.push_back(tangent);
			m_tangents.push_back(tangent);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle[3]);
		glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * sizeof(glm::vec3), m_tangents.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::bindMesh() 
{
	glBindVertexArray(m_vertexArrayHandle);
}

void Mesh::unbindMesh() 
{
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
}


std::vector<std::string> split(std::string str) {
	std::string sep = " ";
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}

std::vector<float> splitF(std::string str) {
	std::string sep = " ";
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<float> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(std::stof(current));
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}

std::vector<int> splitI(std::string str, std::string sep) {
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<int> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(std::stoi(current));
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}

void Mesh::readObj(std::string filename) {
	double a = glfwGetTime();
	std::vector<glm::vec3> V, N;
	std::vector<glm::vec2> T;
	std::vector<int> TempI;
	std::vector<float> TempF;
	std::vector<std::string> TempS;
	std::string line;
	glm::vec3 temp(0.0f);
	glm::vec2 tempt(0.0f);
	int i = 0, j = 0, k = 0, f = 0;
	std::ifstream inFile(RESOURCE_PATH + filename);
	while (getline(inFile, line))
	{
		if (line[0] == 'v') {
			if (line[1] == 't') {
				TempF = splitF(line.substr(3));
				tempt.x = TempF[0];
				tempt.y = TempF[1];
				T.push_back(tempt);
				k++;
			}
			else if (line[1] == 'n') {
				TempF = splitF(line.substr(3));
				temp.x = TempF[0];
				temp.y = TempF[1];
				temp.z = TempF[2];
				N.push_back(temp);
				j++;
			}
			else {
				TempF = splitF(line.substr(2));
				temp.x = TempF[0];
				temp.y = TempF[1];
				temp.z = TempF[2];
				V.push_back(temp);
				i++;
			}
		}
		else if (line[0] == 'f') {
			TempS = split(line.substr(2));
			for (unsigned int i = 0; i < TempS.size(); i++) {
				TempI = splitI(TempS[i], "/");
				if (T.size() != 0) {
					m_vertices.push_back(V[TempI[0] - 1]);
					m_texCoords.push_back(T[TempI[1] - 1]);
					m_normals.push_back(N[TempI[2] - 1]);
				}
				else {
					m_vertices.push_back(V[TempI[0] - 1]);
					m_normals.push_back(N[TempI[1] - 1]);
				}
			}
			f++;
		}
	}
	double b = glfwGetTime();
	inFile.close();
	std::cout <<"Mesh "<< filename << " loaded with: " << i << " vertecies, " << j << " normals, " << k << " texcoords, " << f << " triangles" << std::endl;
	std::cout << b - a << " sec to read file :)" << std::endl;
}