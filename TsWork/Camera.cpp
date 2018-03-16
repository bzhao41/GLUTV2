#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera() {}

void Camera::update(char key) {
	switch (key) {
        case ' ':
            toggleCamera();
            break;
		case 'x': // pitch up
			pitch(1);
			break;
		case 'X': // pitch down
			pitch(-1);
			break;
		case 'z': // roll counterclockwise
			roll(1);
			break;
		case 'Z': // roll clockwise
			roll(-1);
			break;
		case 'c': // yaw counterclockwise
			yaw(-1);
			break;
		case 'C': // yaw clockwise
			yaw(1);
			break;
	}
}

mat4 Camera::getProjectionMatrix() {
	float h = glutGet(GLUT_INIT_WINDOW_HEIGHT);
	float w = glutGet(GLUT_INIT_WINDOW_WIDTH);

	projectionMatrix = Perspective(65, w / h, 1, 100);
	return projectionMatrix;
}

mat4 Camera::getCameraMatrix() {
    if (isCamera1) {
        cameraMatrix = LookAt(eye, eye - n, v);
    } else {
        cameraMatrix = LookAt(vec4(0, 10, 0, 1), vec4(0, 10, 0, 1) - vec4(0, 1, 0, 0), vec4(0, 10, 0, 1) - vec4(0, 0, 1, 0));
    }
    
	return cameraMatrix;
}

mat4 Camera::getViewMatrix() {
	getProjectionMatrix();
	getCameraMatrix();
	return projectionMatrix * cameraMatrix;
}

void Camera::move(int key) {
	switch (key) {
		case 101: // move forward
			eye -= 0.1 * n;
			break;
		case 103: // move backward
			eye += 0.1 * n;
			break;
		case 100: // move left
			eye -= 0.1 * u;
			break;
		case 102: // move right
			eye += 0.1 * u;
			break;
	}

	getCameraMatrix();
	glutPostRedisplay();
}

void Camera::roll(int dir) {
	int degrees = 10;
	vec4 originalU = u;
	vec4 originalV = v;
	
	switch (dir) {
		case 1:  // 'z' - roll counterclockwise
			u = cos(degrees * DegreesToRadians) * originalU - sin(degrees * DegreesToRadians) * originalV;
			v = sin(degrees * DegreesToRadians) * originalU + cos(degrees * DegreesToRadians) * originalV;
			break;
		case -1: // 'Z' - roll clockwise
			degrees *= dir;
			u = cos(degrees * DegreesToRadians) * originalU - sin(degrees * DegreesToRadians) * originalV;
			v = sin(degrees * DegreesToRadians) * originalU + cos(degrees * DegreesToRadians) * originalV;
			break;
	}

	getCameraMatrix();
	glutPostRedisplay();
}

void Camera::pitch(int dir) {
	int degrees = 10;
	vec4 originalV = v;
	vec4 originalN = n;

	switch (dir) {
		case 1:  // 'x' - pitch up
			v = cos(degrees * DegreesToRadians) * originalV - sin(degrees * DegreesToRadians) * originalN;
			n = sin(degrees * DegreesToRadians) * originalV + cos(degrees * DegreesToRadians) * originalN;
			break;
		case -1: // 'X'  pitch down
			degrees *= dir;
			v = cos(degrees * DegreesToRadians) * originalV - sin(degrees * DegreesToRadians) * originalN;
			n = sin(degrees * DegreesToRadians) * originalV + cos(degrees * DegreesToRadians) * originalN;
			break;
	}

	getCameraMatrix();
	glutPostRedisplay();
}

void Camera::yaw(int dir) {
	int degrees = 10;
	vec4 originalU = u;
	vec4 originalN = n;

	switch (dir) {
	case 1:  // 'c' - yaw counterclockwise
		u = cos(degrees * DegreesToRadians) * originalU - sin(degrees * DegreesToRadians) * originalN;
		n = sin(degrees * DegreesToRadians) * originalU + cos(degrees * DegreesToRadians) * originalN;
		break;
	case -1: // 'C' - yaw clockwise
		degrees *= dir;
		u = cos(degrees * DegreesToRadians) * originalU - sin(degrees * DegreesToRadians) * originalN;
		n = sin(degrees * DegreesToRadians) * originalU + cos(degrees * DegreesToRadians) * originalN;
		break;
	}

	getCameraMatrix();
	glutPostRedisplay();
}

void Camera::toggleCamera() {
    isCamera1 = !isCamera1;
}

bool Camera::getCamera() {
    return isCamera1;
}
