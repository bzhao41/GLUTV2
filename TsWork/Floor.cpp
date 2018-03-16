#include "Floor.h"

Floor::Floor() {
	build();
}

Floor::~Floor() { }

void Floor::build() {
	index = 0;
	vec4 points[numVertices] = {
		vec4(-30, 0, -30, 1),
		vec4(-30, 0,  30, 1),
		vec4( 30, 0,  30, 1),
		vec4( 30, 0, -30, 1)
	};

	triangle(points[0], points[1], points[2]);
	triangle(points[2], points[3], points[0]);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	glUseProgram(program);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexNormals), vertexNormals);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
}

void Floor::triangle(vec4 a, vec4 b, vec4 c) {
	vec4 e1 = b - a;
	vec4 e2 = c - a;
	vec3 n = normalize(cross(vec3(e1.x, e1.y, e1.z), vec3(e2.x, e2.y, e2.z)));

	vertexLocations[index] = a;
	vertexNormals[index] = n;
	index++;

	vertexLocations[index] = b;
	vertexNormals[index] = n;
	index++;

	vertexLocations[index] = c;
	vertexNormals[index] = n;
	index++;
}

void Floor::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse  = d;
	matAmbient  = a;
	matSpecular = s;
	shininess   = sh;
}

void Floor::assignParametricNormals() {
	for (int i = 0; i < numVertices; i++) {
		vertexNormals[i] = normalize(vec3(vertexLocations[i].x, vertexLocations[i].y, vertexLocations[i].z));
	}
}

void Floor::draw(Camera* camera, vector<Light> lights) {
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

	glDrawArrays(GL_TRIANGLES, 0, 6);
}