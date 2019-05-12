#include "Teapot.h"

Teapot::Teapot(GLuint ShaderID, std::string albedoname, std::string roughnessname, std::string metalnessname, std::string normalmapname) {
	Tex1ID = glGetUniformLocation(ShaderID, "cubemap");
	Tex2ID = glGetUniformLocation(ShaderID, "albedo");
	Tex3ID = glGetUniformLocation(ShaderID, "roughness");
	Tex4ID = glGetUniformLocation(ShaderID, "metalness");
	Tex5ID = glGetUniformLocation(ShaderID, "normalmap");

	m_albedoname = albedoname;
	m_roughnessname = roughnessname;
	m_metalnessname = metalnessname;
	m_normalmapname = normalmapname;

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(4, VertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_v[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_n[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * _points * sizeof(GL_FLOAT), &_t[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	for (int i = 0; i < _indices; i += 3)
	{
		glm::vec3 vi = glm::vec3(_v[_index[i] * 3], _v[_index[i] * 3 + 1], _v[_index[i] * 3 + 2]);
		glm::vec3 vii = glm::vec3(_v[_index[i + 1] * 3], _v[_index[i + 1] * 3 + 1], _v[_index[i + 1] * 3 + 2]);
		glm::vec3 viii = glm::vec3(_v[_index[i + 2] * 3], _v[_index[i + 2] * 3 + 1], _v[_index[i + 2] * 3 + 2]);

		glm::vec3 deltaPos1 = vi - vii;
		glm::vec3 deltaPos2 = viii - vii;

		glm::vec2 ti = glm::vec2(_t[_index[i] * 2], _t[_index[i] * 2 + 1]);
		glm::vec2 tii = glm::vec2(_t[_index[i + 1] * 2], _t[_index[i + 1] * 2 + 1]);
		glm::vec2 tiii = glm::vec2(_t[_index[i + 2] * 2], _t[_index[i + 2] * 2 + 1]);

		glm::vec2 deltaUV1 = ti - tii;
		glm::vec2 deltaUV2 = tiii - tii;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tmp = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 tangent = glm::vec3(tmp.x, tmp.y, tmp.z);

		_b[_index[i] * 3] = tangent.x;
		_b[_index[i] * 3 + 1] = tangent.y;
		_b[_index[i] * 3 + 2] = tangent.z;
		_b[_index[i + 1] * 3] = tangent.x;
		_b[_index[i + 1] * 3 + 1] = tangent.y;
		_b[_index[i + 1] * 3 + 2] = tangent.z;
		_b[_index[i + 2] * 3] = tangent.x;
		_b[_index[i + 2] * 3 + 1] = tangent.y;
		_b[_index[i + 2] * 3 + 2] = tangent.z;

		//std::cout << tangent.x << " " << tangent.y << " " << tangent.z << std::endl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[3]);
	glBufferData(GL_ARRAY_BUFFER, 3 * _points * sizeof(GL_FLOAT), &_b[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices * sizeof(GL_UNSIGNED_INT), &_index[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::vector<std::string> faces = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	cubemapTexture = TextureTools::loadCubemap(faces);
	albedo = TextureTools::loadTexture(TEXTURE_PATH + m_albedoname);
	roughness = TextureTools::loadTexture(TEXTURE_PATH + m_roughnessname);
	metalness = TextureTools::loadTexture(TEXTURE_PATH + m_metalnessname);
	normalmap = TextureTools::loadTexture(TEXTURE_PATH + m_normalmapname);
}

Teapot::~Teapot() {

}

void Teapot::render() {
	glUniform1i(Tex1ID, 0);
	glUniform1i(Tex2ID, 1);
	glUniform1i(Tex3ID, 2);
	glUniform1i(Tex4ID, 3);
	glUniform1i(Tex5ID, 4);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, metalness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, normalmap);

	glBindVertexArray(VertexArray);
	glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_INT, 0);
}