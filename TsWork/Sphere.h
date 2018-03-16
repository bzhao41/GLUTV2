#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Sphere: public Drawable {
	public:
		Sphere();
		~Sphere();
		vec4* getLocations() { return vertexLocations; }
		void  setMaterial(vec4, vec4, vec4, float);
		void  assignParametricNormals();
		void  draw(Camera*, vector<Light>);
	private:
		static const unsigned int numVertices = 3145728;
		unsigned int index;
		float sqrt2, sqrt6, shininess;
		
        GLuint Texture[10];
        GLuint texLoc;
		vec4 vertexLocations[numVertices];
		vec3 vertexNormals[numVertices];
        vec2 vertexTextures[numVertices];
    
		vec4 matDiffuse, matSpecular, matAmbient;

		void build();
		void tetrahedron(int);
		void divideTriangle(vec4, vec4, vec4, int);
		void triangle(vec4, vec4, vec4);
		vec4 unit(vec4);
};
#endif
