#include "headers/GeometryGenerator.hpp"
#include "GLAD/glad.h"
#include "glm/trigonometric.hpp"
#include "glm/gtc/constants.hpp"

#include <exception>
#include <vector>

GeometryGenerator::GeometryGenerator() {
	m_geometry.clear();
}

GeometryGenerator::compl GeometryGenerator() {
	for (auto elem : m_geometry) {
		glDeleteBuffers(1, &elem.second.vbo);
		glDeleteBuffers(1, &elem.second.ibo);
		glDeleteVertexArrays(1, &elem.second.vao);
	}
	m_geometry.clear();
}

#include <iostream>
Geometry GeometryGenerator::generateGeometry(const char* name, uint location){

	//hexahedron

	//min 1 parallel, 3 wedges (5 vertices total)
	auto vertices = std::vector<float>();
	auto indices = std::vector<uint>();

	auto altitudes = 2, azimuths = 3;
	auto longitudeAngle = glm::pi<float>() / altitudes; //only needs to run half-circle
	auto latitudeAngle = glm::two_pi<float>() / azimuths;

	for (int z = 0; z <= altitudes; ++z) {
		for (int xy = 0; xy < azimuths; ++xy) {
			//add x
			vertices.push_back(glm::cos(xy*latitudeAngle)*glm::sin(z*longitudeAngle));
			//add y
			vertices.push_back(glm::sin(xy*latitudeAngle)*glm::sin(z*longitudeAngle));
			//add z
			vertices.push_back(glm::cos(z * longitudeAngle));
			//add w
			vertices.push_back(1.f); //not necessary, but consistent
		}
	}
	vertices.shrink_to_fit();

	int ref = 0;
	for (auto coord : vertices) {
		std::cout << coord << ",";
		++ref;
		if (ref % 4 == 0) {
			std::cout << std:: endl;
		}
	}

	//indices = { 0, 3, 4, 1, 4, 5, 2, 5, 3, 3, 4, 6, 4, 5, 7, 5, 3, 8 }; //GL_TRIANGLES
	indices = { 0, 3, 4, 6, 5, 3, 0, 4 }; //GL_TRIANGLE_STRIP
	indices.shrink_to_fit();

	Geometry geom = Geometry();
	geom.location = location;
	geom.ibo_sz = indices.size();
	geom.indices = new uint[indices.size()];
	for (int i = 0; i < indices.size(); ++i) {
		geom.indices[i] = indices[i]; //why overrun if they're the same size
	}

	auto vertCoordAmt = 4;
	auto vertSize = indices.size();

	glGenVertexArrays(1, &geom.vao);
	glBindVertexArray(geom.vao);
	glGenBuffers(1, &geom.vbo);

	glBindVertexArray(geom.vao);

	glBindBuffer(GL_ARRAY_BUFFER, geom.vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(location, vertCoordAmt, GL_FLOAT, GL_FALSE, vertCoordAmt * sizeof(float), (void*)(0));
	glEnableVertexArrayAttrib(geom.vao, geom.location);

	glGenBuffers(1, &geom.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_geometry.insert({ name, geom });

	return m_geometry.find(name)->second;
}

bool GeometryGenerator::hasGeometry(const char* name) {
	return m_geometry.find(name) != m_geometry.end();
}

Geometry GeometryGenerator::getGeometry(const char* name) {
	if (auto elem = m_geometry.find(name); elem != m_geometry.end()) {
		return elem->second;
	}
	throw new std::exception("unknown geometry name");
}