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

	theParser.mesh->center = glm::vec3(	(theParser.maxx + theParser.minx)/2.0f, 
										(theParser.maxy + theParser.miny) / 2.0f,
										(theParser.maxz + theParser.minz) / 2.0f);
	theParser.mesh->aabbX = theParser.maxx - theParser.mesh->center.x;
	theParser.mesh->aabbY = theParser.maxy - theParser.mesh->center.y;
	theParser.mesh->aabbZ = theParser.maxz - theParser.mesh->center.z;

	//theParser.mesh->initBuffers();

	return theParser.mesh;
}
bool MeshParser::processLine()
{
	std::string line_id;
	double x, y, z;

	if (!(ifs >> line_id))
		return false;

	if ("v" == line_id) {	//	vertex data
		ifs >> x >> y >> z;
		positions.push_back(glm::vec3(x, y, z));
		mesh->addVertex(glm::vec<3, double>(x, y, z));
		updateMinMaxValues(x,y,z);
	}
	else if ("f" == line_id) {
		unsigned int iPosition0 = 0;//, iTexCoord = 0, iNormal = 0;
		unsigned int iPosition1 = 0;
		unsigned int iPosition2 = 0;
		//for (unsigned int iFace = 0; iFace < 3; iFace++)
		//{
			ifs >> iPosition0;
			ifs >> iPosition1;
			ifs >> iPosition2;
			mesh->addFace({iPosition0, iPosition1, iPosition2});
			
		//}
	}
	else
		skipLine();
	return true;
}

void MeshParser:: updateMinMaxValues(double x, double y, double z) {
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
		std::string line = "v " + std::to_string(mesh.vertices.at(i).x)+" "
			+ std::to_string(mesh.vertices.at(i).y)+" "+std::to_string(mesh.vertices.at(i).z)+"\n";

		ofs.write(line.c_str(), strlen(line.c_str()));
	}
	for (size_t i = 0; i < mesh.faces.size(); i++)
	{
		std::string line = "f " + std::to_string(mesh.faces.at(i)[0]) + " "
			+ std::to_string(mesh.faces.at(i)[1]) + " " + std::to_string(mesh.faces.at(i)[2]) + "\n";

		ofs.write(line.c_str(), strlen(line.c_str()));
	}
	ofs.close();
}


void MeshParser::exportMeshSTL(Mesh* mesh, const char* fileName) {
	/*std::ofstream ofs;
	ofs.open(fileName);
	ofs.write("solid obj\n", 10);
	for (size_t i = 0; i < mesh->faces.size(); i++)
	{
		glm::vec<3, glm::vec<3, double>> triverts = mesh->getVertexesByFace(mesh->faces[i]);
		glm::vec<3, double> normal = mesh->normals[i];

		std::string facet = "facet normal "+ std::to_string(normal.x)+" "+ std::to_string(normal.y)+" "+ std::to_string(normal.z)+"\n";
		ofs.write(facet.c_str(), strlen(facet.c_str()));

		facet = "\touter loop\n";
		ofs.write(facet.c_str(), strlen(facet.c_str()));

		facet = "\t\tvertex "+ std::to_string(triverts[0].x)+ " " + std::to_string(triverts[0].y)+ " " + std::to_string(triverts[0].z)+"\n";
		ofs.write(facet.c_str(), strlen(facet.c_str()));
		facet = "\t\tvertex " + std::to_string(triverts[1].x) + " " + std::to_string(triverts[1].y) + " " + std::to_string(triverts[1].z) + "\n";
		ofs.write(facet.c_str(), strlen(facet.c_str()));
		facet = "\t\tvertex " + std::to_string(triverts[2].x) + " " + std::to_string(triverts[2].y) + " " + std::to_string(triverts[2].z) + "\n";
		ofs.write(facet.c_str(), strlen(facet.c_str()));

		facet = "\tendloop\n";
		ofs.write(facet.c_str(), strlen(facet.c_str()));

		facet = "endfacet\n";
		ofs.write(facet.c_str(), strlen(facet.c_str()));
	}
	ofs.write("endsolid obj\n", 13);
	ofs.close();*/
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