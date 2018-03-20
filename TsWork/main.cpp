#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Cube.h"
#include "Sphere.h"
#include "Floor.h"
#include "SkyBox.hpp"
#include "Particle.hpp"

// Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void keyboardSpecial(int, int, int);
void timer(int);
void animate(int);
void resize(int width, int height);
void close(void);

// Objects
Camera* camera;
Camera* skyCam;
SkyBox* sky;
vector<Light> lights;
vector<Drawable*> drawables;
Particle* particles;

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
	glutTimerFunc(75, timer, 0);

	// Start the main event listening loop
	glutMainLoop();
	return 0;
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	// Create camera
	camera = new Camera();
    skyCam = new Camera(true);
    skyCam->positionCamera(vec4(0, 0, 0, 1), normalize(vec4(0, 0, 2, 1) - vec4(0, 0, 0, 1)), vec4(0, 1, 0, 0));
    
    sky = new SkyBox();
    
    // Create Sun
    Sphere* sun = new Sphere();
    sun->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                     vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                     vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 50);
    sun->setTexture(512, 256, "./sun.ppm");
    drawables.push_back(sun);
    drawables[0]->setModelMatrix(Scale(5, 5, 5));
    
    // Create Mercury
    Sphere* mercury = new Sphere();
    mercury->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                         vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                         vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    mercury->setTexture(1024, 512, "./mercury.ppm");
    mercury->setSpeed(.5);

    drawables.push_back(mercury);
    drawables[1]->setModelMatrix(Translate(0, 0, -7) * Scale(0.2, 0.2, 0.2));
    
    // Create Venus
    Sphere* venus = new Sphere();
    venus->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                       vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                       vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    venus->setTexture(1600, 900, "./venus.ppm");
    drawables.push_back(venus);
    drawables[2]->setModelMatrix(Translate(0, 0, -9) * Scale(0.3, 0.3, 0.3));
    
    // Create Earth
    Sphere* earth = new Sphere();
    earth->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                       vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                       vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    earth->setTexture(1600, 900, "./earth.ppm");
    drawables.push_back(earth);
    drawables[3]->setModelMatrix(Translate(0, 0, -11) * Scale(0.3, 0.3, 0.3));
    
    // Create Mars
    Sphere* mars = new Sphere();
    mars->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                      vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                      vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    mars->setTexture(1024, 512, "./mars.ppm");
    drawables.push_back(mars);
    drawables[4]->setModelMatrix(Translate(0, 0, -13) * Scale(0.25, 0.25, 0.25));
    
    // Create Jupiter
    Sphere* jupiter = new Sphere();
    jupiter->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                         vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                         vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    jupiter->setTexture(1264, 632, "./jupiter.ppm");
    drawables.push_back(jupiter);
    drawables[5]->setModelMatrix(Translate(0, 0, -15) * Scale(0.75, 0.75, 0.75));
    
    // Create Saturn
    Sphere* saturn = new Sphere();
    saturn->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                        vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                        vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    saturn->setTexture(1264, 632, "./saturn.ppm");
    drawables.push_back(saturn);
    drawables[6]->setModelMatrix(Translate(0, 0, -18) * Scale(0.6, 0.6, 0.6));
    
    // Create Uranus
    Sphere* uranus = new Sphere();
    uranus->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                        vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                        vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    uranus->setTexture(1024, 512, "./uranus.ppm");
    drawables.push_back(uranus);
    drawables[7]->setModelMatrix(Translate(0, 0, -20) * Scale(0.4, 0.4, 0.4));
    
    // Create Neptune
    Sphere* neptune = new Sphere();
    neptune->setMaterial(vec4(0.50754f,  0.50754f,  0.50754f,  0.55),
                         vec4(0.19225f,  0.19225f,  0.19225f,  0.55),
                         vec4(0.508273f, 0.508273f, 0.508273f, 0.55), 100);
    neptune->setTexture(1024, 512, "./neptune.ppm");
    neptune->setSpeed(10);
    drawables.push_back(neptune);
    drawables[8]->setModelMatrix(Translate(0, 0, -22) * Scale(0.4, 0.4, 0.4));
    
	// Create floor
//    Floor* mfloor = new Floor();
//    mfloor->setMaterial(vec4(0, 1, 0, 1), vec4(0, 1, 0, 1), vec4(1, 1, 1, 1), 10);
//    drawables.push_back(mfloor);
//    drawables[9]->setModelMatrix(Translate(0, -5, 0));

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
    
    particles = new Particle();
    
    animate(75);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sky->draw(skyCam, lights);
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
	
	//if (y1 <= 0) {
	//	lights[0].turnOff();
	//} else {
	//	lights[0].turnOn();
	//}
	
	glutTimerFunc(75, timer, 0);
	glutPostRedisplay();
}

void animate(int value) {
    int   r[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int   t[] = {7, 9, 11, 13, 15, 18, 20, 22};
    float s[] = {0.2, 0.3, 0.3, 0.25, 0.75, 0.6, 0.4, 0.4};
    static int degrees = 0;
    
    mat4 rot = RotateY(degrees += 5);
    
    drawables[0]->setModelMatrix(rot * Scale(5, 5, 5));
    
    for (int i = 0; i < 8; i++) {
        mat4 t1 = Translate(0, 0, -t[i]);
        mat4 t2 = Translate(0, 0, 0);
        
        drawables[i + 1]->setModelMatrix(t2 * (rot * drawables[i + 1]->getSpeed()) * t1 * Scale(s[i], s[i], s[i]));
    }
    
    glutTimerFunc(50, animate, value);
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
