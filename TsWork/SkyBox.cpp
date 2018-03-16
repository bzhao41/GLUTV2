//
//  SkyBox.cpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/9/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//


#include "SkyBox.hpp"

SkyBox::SkyBox() {
    buildIndexedCube();
    init();
}

SkyBox::~SkyBox(){
    
}

void SkyBox::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4) {
    
    //Triangle 1
    vertexLocations[index] = vertices[ind1];
    index++;
    
    vertexLocations[index] = vertices[ind2];
    index++;
    
    vertexLocations[index] = vertices[ind3];
    index++;
    
    //Triangle 2
    vertexLocations[index] = vertices[ind3];
    index++;
    
    vertexLocations[index] = vertices[ind4];
    index++;
    
    vertexLocations[index] = vertices[ind1];
    index++;
}

void SkyBox::buildIndexedCube() {
    index = 0;
    makeIndexedQuad(1, 0, 3, 2);  //front
    makeIndexedQuad(2, 3, 7, 6);  //right
    makeIndexedQuad(3, 0, 4, 7);  //bottom
    makeIndexedQuad(6, 5, 1, 2);  //top
    makeIndexedQuad(4, 5, 6, 7);  //back
    makeIndexedQuad(5, 4, 0, 1);  //left
}

void SkyBox::init() {
    program = InitShader("./vshaderSky_v150.glsl", "./fshaderSky_v150.glsl");
    glUseProgram(program); //make it the active one
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //associate the VBO with the active VAO
    
    //make this VAO active
    glGenBuffers(1, &VBO);
    //put the data on the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
    
    //set up stuff for the body of the cube
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition); //enable it
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    //skybox
    
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &Texture[0]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture[0]);
    
    
    int imgsize = 512;
    
    GLubyte *top = ppmRead("./sky-top.ppm", &imgsize, &imgsize);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, top);
    
    GLubyte *bottom = ppmRead("./sky-bottom.ppm", &imgsize, &imgsize);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom);
    
    GLubyte *front = ppmRead("./sky-front.ppm", &imgsize, &imgsize);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, front);
    
    GLubyte *back = ppmRead("./sky-back.ppm", &imgsize, &imgsize);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, back);
    
    GLubyte *left = ppmRead("./sky-left.ppm", &imgsize, &imgsize);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, left);
    
    GLubyte *right = ppmRead("./sky-right.ppm", &imgsize, &imgsize);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, right);
    
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    delete[] top;
    delete[] bottom;
    delete[] left;
    delete[] back;
    delete[] right;
    delete[] front;
    
}

void SkyBox::draw(Camera* cam, vector<Light> lights){
    glBindVertexArray(VAO);
    glUseProgram(program); //also switch to using this shader program
    
    GLuint vmLoc = glGetUniformLocation(program, "view_matrix");
    GLuint pmLoc = glGetUniformLocation(program, "proj_matrix");
    
    glUniformMatrix4fv(vmLoc, 1, GL_TRUE, cam->getViewMatrix());
    glUniformMatrix4fv(pmLoc, 1, GL_TRUE, cam->getProjectionMatrix());
    
    
    glEnable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture[0]);
    texLoc = glGetUniformLocation(program, "textureID");
    glUniform1i(texLoc, 0);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
}

void SkyBox::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
    matDiffuse  = d;
    matAmbient  = a;
    matSpecular = s;
    shininess   = sh;
}


