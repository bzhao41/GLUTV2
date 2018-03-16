#include "Cube.h"

Cube::Cube() {
	buildCube();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	glUseProgram(program);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(vertexNormals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(vertexNormals), vertexNormals);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
}

Cube::~Cube() {}

//----------------------------------------------------------------------------
void Cube::makeQuad(int ind1, int ind2, int ind3, int ind4) {
	// Triangle #1
	vec4 e1 = vertices[ind2] - vertices[ind1];
	vec4 e2 = vertices[ind3] - vertices[ind1];
	vec3 n1 = normalize(cross(vec3(e1.x, e1.y, e1.z), vec3(e2.x, e2.y, e2.z)));

	points[index] = vertices[ind1];
	colors[index] = potentialColors[index % 8];
	vertexNormals[index] = n1;
	index++;

	points[index] = vertices[ind2];
	colors[index] = colors[index - 1];
	vertexNormals[index] = n1;
	index++;

	points[index] = vertices[ind3];
	colors[index] = colors[index - 1];
	vertexNormals[index] = n1;
	index++;

	// Triangle #2
	vec4 e3 = vertices[ind4] - vertices[ind3];
	vec4 e4 = vertices[ind1] - vertices[ind3];
	vec3 n2 = normalize(cross(vec3(e3.x, e3.y, e3.z), vec3(e4.x, e4.y, e4.z)));

	points[index] = vertices[ind3];
	colors[index] = colors[index - 1];
	vertexNormals[index] = n2;
	index++;

	points[index] = vertices[ind4];
	colors[index] = colors[index - 1];
	vertexNormals[index] = n2;
	index++;

	points[index] = vertices[ind1];
	colors[index] = colors[index - 1];
	vertexNormals[index] = n2;
	index++;
}

void Cube::buildCube() {
	index = 0;

	makeQuad(1, 0, 3, 2);  //front
	makeQuad(2, 3, 7, 6);  //right
	makeQuad(3, 0, 4, 7);  //bottom
	makeQuad(6, 5, 1, 2);  //top
	makeQuad(4, 5, 6, 7);  //back
	makeQuad(5, 4, 0, 1);  //left

	assignParametricNormals();
}

void Cube::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse  = d;
	matAmbient  = a;
	matSpecular = s;
	shininess   = sh;
}

void Cube::assignParametricNormals() {
	for (int i = 0; i < 6; i++) {
		vertexNormals[i] = normalize(vec3(points[i].x, points[i].y, points[i].z));
	}
}

void Cube::draw(Camera* camera, vector<Light> lights) {
	glBindVertexArray(VAO);
	glUseProgram(program);

	GLuint mmLoc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(mmLoc, 1, GL_TRUE, modelmatrix);
	GLuint vmLoc = glGetUniformLocation(program, "view_matrix");
	glUniformMatrix4fv(vmLoc, 1, GL_TRUE, camera->getViewMatrix());
	GLuint pmLoc = glGetUniformLocation(program, "proj_matrix");
	glUniformMatrix4fv(pmLoc, 1, GL_TRUE, camera->getProjectionMatrix());
	GLuint cmLoc = glGetUniformLocation(program, "cam_matrix");
	glUniformMatrix4fv(cmLoc, 1, GL_TRUE, camera->getCameraMatrix());

	// Get shader variable locations and set uniform variables
	GLuint mAlphaLoc = glGetUniformLocation(program, "matAlpha");
	GLuint maLoc = glGetUniformLocation(program, "matAmbient");
	GLuint mdLoc = glGetUniformLocation(program, "matDiffuse");
	GLuint msLoc = glGetUniformLocation(program, "matSpecular");
	GLuint enabled1 = glGetUniformLocation(program, "enabled1");
	GLuint enabled2 = glGetUniformLocation(program, "enabled2");
	glUniform1f(mAlphaLoc, shininess);
	glUniform4fv(maLoc, 1, matAmbient);
	glUniform4fv(mdLoc, 1, matDiffuse);
	glUniform4fv(msLoc, 1, matSpecular);
	glUniform1i(enabled1, lights[0].getEnabled());
	glUniform1i(enabled2, lights[1].getEnabled());

	// Light #1, directional light
	GLuint lpLoc1 = glGetUniformLocation(program, "lightPos1");
	GLuint laLoc1 = glGetUniformLocation(program, "lightAmbient1");
	GLuint ldLoc1 = glGetUniformLocation(program, "lightDiffuse1");
	GLuint lsLoc1 = glGetUniformLocation(program, "lightSpecular1");
	glUniform4fv(lpLoc1, 1, lights[0].getPosition());
	glUniform4fv(laLoc1, 1, lights[0].getAmbient());
	glUniform4fv(ldLoc1, 1, lights[0].getDiffuse());
	glUniform4fv(lsLoc1, 1, lights[0].getSpecular());

	// Light #2, spotlight
	GLuint lpLoc2 = glGetUniformLocation(program, "lightPos2");
	GLuint laLoc2 = glGetUniformLocation(program, "lightAmbient2");
	GLuint ldLoc2 = glGetUniformLocation(program, "lightDiffuse2");
	GLuint lsLoc2 = glGetUniformLocation(program, "lightSpecular2");
	glUniform4fv(lpLoc2, 1, lights[1].getPosition());
	glUniform4fv(laLoc2, 1, lights[1].getAmbient());
	glUniform4fv(ldLoc2, 1, lights[1].getDiffuse());
	glUniform4fv(lsLoc2, 1, lights[1].getSpecular());

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
