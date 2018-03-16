#ifndef __CUBE_H__
#define __CUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Cube: public Drawable {
	public:
		Cube();
		~Cube();
		vec4* getLocations() { return vertices; }
		void  setMaterial(vec4, vec4, vec4, float);
		void  assignParametricNormals();
		void  draw(Camera*, vector<Light>);

	private:
		static const unsigned int numVertices = 36;
		unsigned int index;
		float shininess;

		vec3 vertexNormals[36];
		vec4 matDiffuse, matSpecular, matAmbient;
		vec4 points[36];
		vec4 colors[36];

		vec4 vertices[8] = { 
			vec4(-0.5, -0.5,  0.5, 1),
			vec4(-0.5,  0.5,  0.5, 1),
			vec4( 0.5,  0.5,  0.5, 1),
			vec4( 0.5, -0.5,  0.5, 1),
			vec4(-0.5, -0.5, -0.5, 1),
			vec4(-0.5,  0.5, -0.5, 1),
			vec4( 0.5,  0.5, -0.5, 1),
			vec4( 0.5, -0.5, -0.5, 1) 
		};
		vec4 potentialColors[8] = { 
			vec4(0, 0, 1, 1), // top
			vec4(0, 0, 1, 1), 
			vec4(0, 0, 1, 1), 
			vec4(0, 0, 1, 1),
			vec4(0, 0, 1, 1), 
			vec4(0, 0, 1, 1),
			vec4(0, 0, 1, 1),
			vec4(0, 0, 1, 1) 
		};

		void buildCube();
		void makeQuad(int, int, int, int);
};
#endif
