#include "Sphere.h"

Sphere::Sphere() {
	build();
}

Sphere::~Sphere() { }

void Sphere::build() {
	sqrt2 = (float)sqrt(2.0);
	sqrt6 = (float)sqrt(6.0);
	index = 0;

	tetrahedron(9);
	assignParametricNormals();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl");
	glUseProgram(program);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals) + sizeof(vertexTextures), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexNormals), vertexNormals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals), sizeof(vertexTextures), vertexTextures);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));
    
    GLuint vTexture = glGetAttribLocation(program, "vTexture");
    glEnableVertexAttribArray(vTexture);
    glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations) + sizeof(vertexNormals)));
}

void Sphere::tetrahedron(int count) {
	vec4 v[4] = {
		vec4(0, 0, 1, 1),
		vec4(0, 2 * sqrt2 / 3, -1.0f / 3.0f, 1),
		vec4(-sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f, 1.0f),
		vec4( sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f, 1.0f)
	};

	divideTriangle(v[0], v[1], v[2], count);
	divideTriangle(v[3], v[2], v[1], count);
	divideTriangle(v[0], v[3], v[1], count);
	divideTriangle(v[0], v[2], v[3], count);
}

void Sphere::divideTriangle(vec4 a, vec4 b, vec4 c, int count) {
	if (count > 0) {
		vec4 v1 = unit(a + b);
		v1.w = 1.0;
		vec4 v2 = unit(a + c);
		v2.w = 1.0;
		vec4 v3 = unit(b + c);
		v3.w = 1.0;

		divideTriangle(a,  v1, v2, count - 1);
		divideTriangle(c,  v2, v3, count - 1);
		divideTriangle(b,  v3, v1, count - 1);
		divideTriangle(v1, v3, v2, count - 1);
	} else {
		triangle(a, b, c);
	}
}

void Sphere::triangle(vec4 ai, vec4 bi, vec4 ci) {
    vec3 a = vec3(ai.x, ai.y, ai.z);
    vec3 b = vec3(bi.x, bi.y, bi.z);
    vec3 c = vec3(ci.x, ci.y, ci.z);
    
    float u, v;
    
    vec3 N = normalize(cross(b - a, c - a));
    
    vertexLocations[index] = a;
    vertexNormals[index] = N;
    u = .5 + asin(a.x)/M_PI;
    v = .5 + asin(a.y)/M_PI;
    vertexTextures[index] = vec2(u, -v);
    index++;
    
    vertexLocations[index] = b;
    vertexNormals[index] = N;
    u = .5 + asin(b.x)/M_PI;
    v = .5 + asin(b.y)/M_PI;
    vertexTextures[index] = vec2(u, -v);
    index++;
    
    vertexLocations[index] = c;
    vertexNormals[index] = N;
    u = .5 + asin(c.x)/M_PI;
    v = .5 + asin(c.y)/M_PI;
    vertexTextures[index] = vec2(u, -v);
    index++;
}

vec4 Sphere::unit(vec4 p) {
	float len = p.x * p.x + p.y * p.y + p.z * p.z;
	vec4 t;

	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

void Sphere::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse  = d;
	matAmbient  = a;
	matSpecular = s;
	shininess   = sh;
}

void Sphere::setTexture(int w, int h, char* texture) {
    int width  = w;
    int height = h;
    GLubyte *data = ppmRead(texture, &width, &height);
    glGenTextures(1, &Texture[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[0]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    delete[] data;
}

void Sphere::assignParametricNormals() {
	for (int i = 0; i < numVertices; i++) {
		vertexNormals[i].x = vertexLocations[i].x;
		vertexNormals[i].y = vertexLocations[i].y;
		vertexNormals[i].z = vertexLocations[i].z;
	}
}

void Sphere::draw(Camera* camera, vector<Light> lights) {
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
    
    glUniform1i(texLoc, 0);

	// Get shader variable locations and set uniform variables
	GLuint mAlphaLoc = glGetUniformLocation(program, "matAlpha");
	GLuint maLoc = glGetUniformLocation(program, "matAmbient");
	GLuint mdLoc = glGetUniformLocation(program, "matDiffuse");
	GLuint msLoc = glGetUniformLocation(program, "matSpecular"); 
	GLuint enabled1 = glGetUniformLocation(program, "enabled1");
	GLuint enabled2 = glGetUniformLocation(program, "enabled2");
    GLuint texLoc = glGetUniformLocation(program, "textureID");
    
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
	
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[0]);
    
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
