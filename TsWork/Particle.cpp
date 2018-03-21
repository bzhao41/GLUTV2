//
//  Particles.cpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/15/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle() {
    initializeParticles();
}

void Particle::initializeParticles() {
    for (int i = 0; i < numParticles; i++) {
        particles[i].mass = 1.0;
        for (int j = 0; j < 3; j++) {
            particles[i].color[j] = (float) rand()/RAND_MAX;
            particles[i].position[j] = 2.0 * ((float) rand()/RAND_MAX) - 1.0;
            particles[i].velocity[j] =2.0 * ((float) rand()/RAND_MAX) - 1.0;
        }
        particles[i].color.w = 1.0;
        particles[i].position.w = 1.0;
        particles[i].velocity.w = 1.0;
    }
    
    for (int i = 0; i < numParticles; i++) {
        particlePoints[i] = particles[i].position;
        particleColors[i] = particles[i].color;
        particleVelocities[i] = particles[i].velocity;
    }
    
    program = InitShader("./vshaderParticle_v150.glsl", "./fshaderParticle_v150.glsl");
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    
    glBindVertexArray(VAO);
    glUseProgram(program);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particlePoints) + sizeof(particleColors) + sizeof(particleVelocities), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particlePoints), particlePoints);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints), sizeof(particleColors), particleColors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(particlePoints) + sizeof(particleColors), sizeof(particleVelocities), particleVelocities);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(particlePoints)));
    
    GLuint vVelocity = glGetAttribLocation(program, "vVelocity");
    glEnableVertexAttribArray(vVelocity);
    glVertexAttribPointer(vVelocity, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(particlePoints) + sizeof(particleColors)));
    
}

float Particle::applyForces(int i, int j){
    if(j==1) //only affect y direction
        return -1;
    else
        return 0;
}

float coef; //how strong it bounces back
void Particle::testCollision(int n){
    for(int i=0;i<3; i++){
        if(particles[n].position[i] >=1.0){
            particles[n].velocity[i] *= -coef;
            particles[n].position[i] = 1.0-coef*(particles[n].position[i]-1.0);
        }
        if(particles[n].position[i]<=-1.0){
            particles[n].velocity[i] *= -coef;
            particles[n].position[i] = -1.0-coef*(particles[n].position[i]+1.0);
        }
    }
}

void Particle::draw(Camera* cam, vector<Light> lights) {
    glBindVertexArray(VAO);
    glUseProgram(program);
    
    GLuint mmLoc = glGetUniformLocation(program, "model_matrix");
    GLuint vmLoc = glGetUniformLocation(program, "view_matrix");
    GLuint pmLoc = glGetUniformLocation(program, "proj_matrix");
    
    glUniformMatrix4fv(mmLoc, 1, GL_TRUE, modelmatrix);
    glUniformMatrix4fv(vmLoc, 1, GL_TRUE, cam->getViewMatrix());
    glUniformMatrix4fv(pmLoc, 1, GL_TRUE, cam->getProjectionMatrix());
    
    glPointSize(3.0);
    glDrawArrays(GL_POINTS, 0, numParticles);
    
    glBindVertexArray(0);
    }

float lastTime = 0;
void Particle::updateParticles() {
    float dt;
    float presentTime = glutGet(GLUT_ELAPSED_TIME);

    dt = 0.001 * (presentTime - lastTime);
    float percentFlock = 0.01;
    vec4 ap = getAveragePosition();

    for (int i = 0; i < numParticles; i++) {
        vec4 flockVec = ap - particles[i].position;
        particles[i].velocity = (1.0 - percentFlock) * particles[i].velocity + percentFlock * flockVec;
        for (int j = 0; j < 3; j++) {
            particles[i].position[j] += dt * particles[i].velocity[j];
            particles[i].velocity[j] += dt * applyForces(i, j)/particles[i].mass;
        }
        testCollision(1);
    }
    lastTime = presentTime;
//    float elapsedTime = 0.001 * glutGet(GLUT_ELAPSED_TIME);
//    glUseProgram(program);
//    GLuint time = glGetAttribLocation(program, "time");
//    glUniform1f(time, elapsedTime);
    
}

vec4 Particle::getAveragePosition() {
    vec4 cm = vec4(1.0);
    for (int i = 0 ; i < 3; i++) {
        cm[i] = 0.0;
        for (int j = 0; j < numParticles; j++) {
            cm[i] += particles[j].position[i];
        }
        cm[i] /= numParticles;
    }
    return cm;
}

void Particle::idle(int x) {
    updateParticles();
    glutPostRedisplay();
}

