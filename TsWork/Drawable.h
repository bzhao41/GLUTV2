#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include <cassert>
#include <vector>

using namespace std;

class Drawable {
	protected:
		GLuint program;
		GLuint VAO;
		GLuint VBO;
		mat4 modelmatrix;
    float speed = 1;
	public:
		virtual void draw(Camera*, vector<Light>) = 0;
		virtual vec4* getLocations() = 0;
		mat4 getModelMatrix() { return modelmatrix; }
		void setModelMatrix(mat4 mm) { modelmatrix = mm; }
		static unsigned char* ppmRead(char* filename, int* width, int* height);
		virtual ~Drawable() = 0;;
    void  setSpeed(float s) {speed = s;};
    float getSpeed() {return speed; };

};
#endif
