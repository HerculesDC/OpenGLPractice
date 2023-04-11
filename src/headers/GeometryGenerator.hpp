/******************************************************************************
 * Geometry Generator class:                                                  *
 * Generates simple geometry,does not keep track of it. However, it does keep *
 * track of the GL-related attributes (such as location, buffer, index and    *
 * array objects. Does not handle textures (for now).                         * 
 *****************************************************************************/

#ifndef GEOMETRYGENERATOR_HPP
#define GEOMETRYGENERATOR_HPP
#include <map>

typedef unsigned int uint;

struct Geometry {
	uint vao, vbo, ibo, location;
	size_t ibo_sz;
	uint* indices;
};

class GeometryGenerator {
	public:
		GeometryGenerator();
		compl GeometryGenerator();
	public:
		Geometry generateGeometry(const char*, uint); //name, location
		bool hasGeometry(const char*);
		Geometry getGeometry(const char*);

	private:
		std::map<const char*, Geometry> m_geometry;
};

#endif