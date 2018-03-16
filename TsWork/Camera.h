#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Angel.h"

class Camera {
	private:
		bool isCamera1 = true;

	public:
		Camera();
		void update(char);
		void toggleCamera();
		bool getCamera();

		// Projection matrix
		mat4 getProjectionMatrix();
		mat4 projectionMatrix;

		// Camera matrix
		mat4 getCameraMatrix();
		mat4 cameraMatrix;

		// Final view matrix
		mat4 getViewMatrix();
		mat4 viewMatrix;

		// Camera variables
		vec4 eye = vec4(0, 2, 0, 1);
		vec4 u   = vec4(1, 0, 0, 0);
		vec4 v   = vec4(0, 1, 0, 0);
		vec4 n   = vec4(0, 0, 1, 0);

		void move(int);
		void roll(int);
		void pitch(int);
		void yaw(int);
};
#endif
