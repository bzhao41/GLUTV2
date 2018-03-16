#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "Angel.h"

class Light {
	public:
		Light(vec4 p, vec4 a, vec4 s, vec4 d) : position(p), ambient(a), specular(s), diffuse(d) {}
		vec4 getPosition() { return position; }
		vec4 getAmbient()  { return ambient; }
		vec4 getDiffuse()  { return diffuse; }
		vec4 getSpecular() { return specular; }

		void setPosition(vec4 pos) { position = pos; }
		void turnOn()     { enabled = 1; }
		void turnOff()    { enabled = 0; }
		int  getEnabled() { return enabled; }
	private:
		vec4 position, ambient, diffuse, specular;
		int enabled;
};
#endif
