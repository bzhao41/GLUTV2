#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Cube.h"
#include "Sphere.h"
#include "Floor.h"

// Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void keyboardSpecial(int, int, int);
void timer(int);
void resize(int width, int height);
void close(void);

// Objects
Camera* camera;
vector<Light> lights;
vector<Drawable*> drawables;
bool toggleFlashlight = false;
static float t = 0;

GLuint windowID = 0;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	#ifdef __APPLE__
		glutInitDisplayMode(GLUT_DEPTH | GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE);
	#else
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	#endif
		glutInitWindowSize(512, 512);

	windowID = glutCreateWindow("CS 432 - Homework 5");

	// Initialize glew on non-apple systems
	#ifndef __APPLE__
		GLenum err = glewInit();
	#endif

	init();

	// Set up the callback functions
	glutDisplayFunc(display);
	glutWMCloseFunc(close);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutTimerFunc(50, timer, 0);

	// Start the main event listening loop
	glutMainLoop();
	return 0;
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	// Create camera
	camera = new Camera();

	// Create sphere
	Sphere* msphere = new Sphere();
    msphere->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                         vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                         vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
	drawables.push_back(msphere);

	// Create cube
	Cube* mcube = new Cube();
	mcube->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  1),
					   vec4(0.19225f,  0.19225f,  0.19225f,  1), 
					   vec4(0.508273f, 0.508273f, 0.508273f, 1), 40);
	drawables.push_back(mcube);

	// Create floor
	Floor* mfloor = new Floor();
	mfloor->setMaterial(vec4(0, 1, 0, 1), vec4(0, 1, 0, 1), vec4(1, 1, 1, 1), 10);
	drawables.push_back(mfloor);

	drawables[0]->setModelMatrix(Translate(0, 1, -4));
	drawables[1]->setModelMatrix(Translate(2, 1, -5));

	// Create directional light
	Light l1 = Light({ 1, 0, 0, 0 }, 
					 { 1.0f, 1.0f, 1.0f, 1.0f }, 
					 { 1.0f, 1.0f, 1.0f, 1.0f }, 
					 { 1.0f, 1.0f, 1.0f, 1.0f });
	l1.turnOn();

	// Create spotlight
	Light l2 = Light(camera->eye, 
					 { 1.0f, 1.0f, 1.0f, 1.0f }, 
					 { 1.0f, 1.0f, 1.0f, 1.0f }, 
					 { 1.0f, 1.0f, 1.0f, 1.0f });

	lights.push_back(l1);
	lights.push_back(l2);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < drawables.size(); i++) {
		drawables[i]->draw(camera, lights);
	}

	glutSwapBuffers();
}

void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
}

void toggle() {
	toggleFlashlight = !toggleFlashlight;

	if (toggleFlashlight) {
		lights[1].turnOn();
	}
	else {
		lights[1].turnOff();
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'f':
			toggle();
			break;
        case ' ':
		case 'p': 
		case 'P':
		case 'x': 
		case 'X':
		case 'z': 
		case 'Z':
		case 'c': 
		case 'C':
			camera->update(key);
			break;
		case 033: // Escape key
		case 'q': case 'Q':
			close();
			break;
	}
}

void keyboardSpecial(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT:
			camera->move(key);
			break;
	}
}

void timer(int x) {
	vec4 direction = lights[0].getPosition();
	float x1 = direction.x;
	float y1 = direction.y;

	x1 = cos(t) * 10;
	y1 = sin(t) * 10;
	t += 0.01;

	lights[0].setPosition(vec4(x1, y1, 0, 1));
	
	if (y1 <= 0) {
		lights[0].turnOff();
	} else {
		lights[0].turnOn();
	}
	
	glutTimerFunc(50, timer, 0);
	glutPostRedisplay();
}

void close() {
	for (unsigned int i = 0; i < drawables.size(); i++) {
		delete(drawables[i]);
	}

	if (windowID > 0) {
		glutDestroyWindow(windowID);
	}
	
    exit(EXIT_SUCCESS);
}
