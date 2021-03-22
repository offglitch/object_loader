#pragma once
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

// unsigned int loadModel(std::string& path)
// Load the .obj file in glList
// returns glList.
// On error it returns 0
unsigned int loadModel(std::string& path) {

	int objectCount = 0;
	std::string line;
	char obj_name[512];
	char ch;

	std::ifstream f;

	std::vector<glm::vec3> vertices, normals;

	float diffuseMaterial[4] = { 1.0, 1.0, 1.0, 1.0 };
	float ambientMaterial[4] = { 1.0, 1.0, 1.0, 1.0 };
	float specularMaterial[4] = { 1.0, 1.0, 1.0, 1.0 };
	float none[4] = { 0.0, 0.0, 0.0, 0.0 };

	// Genrate List and get listId
	unsigned int id = glGenLists(1);
	glNewList(id, GL_COMPILE);

	// Open file and check good to read
	f.open(path, std::ifstream::in);

	if (f.good())
		printf("%s: good\n", path.c_str());
	else
		return 0;

	// Parse Object file
	while (f.good() && std::getline(f, line)) {
		glm::vec3 v;
		if (sscanf_s(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z) == 3) {
			// vertices
			vertices.push_back(v);
		} else if (sscanf_s(line.c_str(), "vn %f %f %f", &v.x, &v.y, &v.z) == 3) {
			// normals
			normals.push_back(v);
		} else if(sscanf_s(line.c_str(), "g %s", obj_name, 512) == 1) {
			// object
			objectCount++;
		} else if((sscanf_s(line.c_str(), "%c ", &ch, 1)) == 1 && ch == 'f') {
			// face
			size_t f;
			//printf("char %c\n", ch);
			char* token, *next_token = NULL;
			char temp_line[512];
			std::copy(line.begin(), line.end(), temp_line);
			token = strtok_s(temp_line, " ", &next_token);

			// Apply the material and color to faces
			ambientMaterial[0] = diffuseMaterial[0] * 0.2f;
			ambientMaterial[1] = diffuseMaterial[1] * 0.2f;
			ambientMaterial[2] = diffuseMaterial[2] * 0.2f;
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, none);
			glColor4f(diffuseMaterial[0], diffuseMaterial[1], diffuseMaterial[2], diffuseMaterial[3]);

			// Penerate polygon from vertix index
			glBegin(GL_POLYGON);

			while (token != NULL) {
				f = atoi(token);
				if (f != 0) {
					//printf(" %d %f %f %f\n", f,points[f - 1].x, points[f - 1].y, points[f - 1].z);
					if (normals.size() > 0 && f - 1 < normals.size()) // normals available
						glNormal3f(normals[f - 1].x, normals[f - 1].y, normals[f - 1].z); // load normals
					glVertex3f(vertices[f - 1].x, vertices[f - 1].y, vertices[f - 1].z); // load vertices
				}
				token = strtok_s(NULL, " ", &next_token);
			}

			glEnd();
		} else {
			// Not handled
		}
	}

	glEndList();

	// Clear the vertices and normals as loaded into gpu
	// they are not needed any more
	vertices.clear();
	normals.clear();

	// Close file
	f.close();

	return id;
}