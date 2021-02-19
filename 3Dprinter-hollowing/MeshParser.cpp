#include <string>
#include "MeshParser.h"

Mesh* MeshParser::parse(const char* fileName)
{
	MeshParser theParser;

	theParser.ifs.open(fileName, std::ios::in | std::ios::binary);
	if (!theParser.ifs)
		throw(EXC_FILENOTFOUND);

	theParser.mesh = new Mesh();

	while (theParser.skipCommentLine())
	{
		if (false == theParser.processLine())
			break;
	}
	theParser.ifs.close();

	theParser.mesh->center = glm::vec3((theParser.maxx + theParser.minx)/2.0f, 
										(theParser.maxy + theParser.miny) / 2.0f,
										(theParser.maxz + theParser.minz) / 2.0f);

	//theParser.mesh->initBuffers();

	return theParser.mesh;
}
bool MeshParser::processLine()
{
	std::string line_id;
	float x, y, z;

	if (!(ifs >> line_id))
		return false;

	if ("v" == line_id) {	//	vertex data
		ifs >> x >> y >> z;
		positions.push_back(glm::vec3(x, y, z));
		mesh->addVertex(glm::vec3(x, y, z));
		updateMinMaxValues(x,y,z);
	}
	else if ("f" == line_id) {
		unsigned int iPosition = 0, iTexCoord = 0, iNormal = 0;

		for (unsigned int iFace = 0; iFace < 3; iFace++)
		{
			ifs >> iPosition;
			mesh->addIndex(iPosition);
		}
	}
	else
		skipLine();
	return true;
}

void MeshParser:: updateMinMaxValues(float x, float y, float z) {
		if (x < minx || firstVertex) minx = x;
		if (x > maxx || firstVertex) maxx = x;
		if (y < miny || firstVertex) miny = y;
		if (y > maxy || firstVertex) maxy = y;
		if (z < minz || firstVertex) minz = z;
		if (z > maxz || firstVertex) maxz = z;
		if (firstVertex) firstVertex = false;

}
void MeshParser::exportMesh(const Mesh& mesh, const char* filename) {
	std::ofstream ofs;
	ofs.open(filename);
	for (size_t i = 0; i < mesh.vertices.size(); i++)
	{
		std::string v= "v ";
		std::string line = "v " + std::to_string(mesh.vertices.at(i).x)+" "
			+ std::to_string(mesh.vertices.at(i).y)+" "+std::to_string(mesh.vertices.at(i).z)+"\n";

		ofs.write(line.c_str(), strlen(line.c_str()));
	}
	for (size_t i = 0; i < mesh.indices.size(); i+=3)
	{
		std::string line = "f " + std::to_string(mesh.indices.at(i)) + " "
			+ std::to_string(mesh.indices.at(i+1)) + " " + std::to_string(mesh.indices.at(i+2)) + "\n";

		ofs.write(line.c_str(), strlen(line.c_str()));
	}
	ofs.close();
}




bool MeshParser::skipCommentLine()
{
	char next;
	while (ifs >> std::skipws >> next)
	{
		ifs.putback(next);
		if ('#' == next)
			skipLine();
		else
			return true;
	}
	return false;
}

void MeshParser::skipLine()
{
	char next;
	ifs >> std::noskipws;
	while ((ifs >> next) && ('\n' != next));
}