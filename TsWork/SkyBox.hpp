//
//  SkyBox.hpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/9/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//


#ifndef SkyBox_hpp
#define SkyBox_hpp

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class SkyBox: public Drawable{
public:
    SkyBox();
    ~SkyBox();
    void draw(Camera*, vector<Light>);
    void setMaterial(vec4, vec4, vec4, float);
    vec4* getLocations() { return vertexLocations; }
    
private:
    void init();
    void buildIndexedCube();
    void makeIndexedQuad(GLubyte, GLubyte, GLubyte, GLubyte);
    float shininess;
    vec4 matDiffuse, matSpecular, matAmbient;
    unsigned int index;
    GLuint Texture[10];
    GLuint texLoc;
    
    vec4 vertices[8] = { vec4(-1, -1, 1, 1),vec4(-1,1,1,1),vec4(1,1,1,1),vec4(1,-1,1,1),
        vec4(-1,-1,-1,1),vec4(-1, 1,-1,1),vec4(1,1,-1,1),vec4(1,-1,-1,1) };
    
    vec4 vertexLocations[6 * 2 * 3];
};

#endif /* SkyBox_hpp */




