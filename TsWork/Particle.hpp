//
//  Particles.hpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/15/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#ifndef Particles_hpp
#define Particles_hpp

#include <stdio.h>
#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>
#include <math.h>
class Particle : public Drawable {
public:
    struct particle{
        vec4 color;
        vec4 position;
        vec4 velocity;
        float mass;
    };
    Particle();
    void initializeParticles();
    void updateParticles();
    void draw(Camera*, vector<Light>);
    vec4* getLocations() {return particlePoints; };
    float applyForces(int, int);
    void testCollision(int);
    vec4 getAveragePosition();
    void idle(int);
private:
static const int numParticles = 1000;
particle particles[numParticles];
vec4 particlePoints[numParticles];
vec4 particleColors[numParticles];
vec4 particleVelocities[numParticles];
//
//    static const int ParticleCount = 500;
//    GLfloat texture[10];
//    PARTICLES Particle[ParticleCount];


};
#endif /* Particles_hpp */

