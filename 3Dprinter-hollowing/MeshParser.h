#pragma once

#include <fstream>
#include <map>
#include "Mesh.h"

class MeshParser
{
public:
	static Mesh* parse(const char* fileName);

	static void exportMesh(const Mesh& mesh, const char* fileName);

	enum Exception { EXC_FILENOTFOUND };
private:
	struct IndexedVert {
		int v, vt, vn;
		IndexedVert(int _v, int _vt, int _vn) : v(_v), vt(_vt), vn(_vn) {};
		bool operator<(const IndexedVert& rhs) const {
			return v < rhs.v || (v == rhs.v && (vt < rhs.vt || (vt == rhs.vt && vn < rhs.vn)));
		}
	};

	MeshParser() : mesh(0), nIndexedVerts(0) {}

	bool processLine();
	bool skipCommentLine();
	void skipLine();
	void updateMinMaxValues(float x, float y, float z);

	Mesh* mesh;
	std::ifstream ifs;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;

	unsigned int nIndexedVerts;
	std::map<IndexedVert, unsigned int> vertexIndices;

	float minx = 0, miny = 0, minz = 0, maxx = 0, maxy = 0, maxz = 0;
	bool firstVertex = true;
};