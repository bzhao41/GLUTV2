#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Floor: public Drawable {
	public:
		Floor();
		~Floor();
		vec4* getLocations() { return vertexLocations; }
		void  setMaterial(vec4, vec4, vec4, float);
		void  assignParametricNormals();
		void  draw(Camera*, vector<Light>);
	private:
		static const unsigned int numVertices = 4;
		unsigned int index;
		float shininess;

		vec4 vertexLocations[6];
		vec3 vertexNormals[numVertices];
		vec4 matDiffuse, matSpecular, matAmbient;

		void build();
		void triangle(vec4, vec4, vec4);
};
#endif
