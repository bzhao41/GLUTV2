#version 150

in vec4 vPosition;
in vec3 vNormal;

out vec3 fN1, fE1, fL1;
out vec3 fN2, fE2, fL2;

uniform mat4 model_matrix;
uniform mat4 view_matrix, proj_matrix, cam_matrix;

uniform vec4 lightPos1, lightPos2;

void main() {
	vec3 posInCam1   = (cam_matrix * model_matrix * vPosition).xyz;
	vec3 lightInCam1 = (cam_matrix * lightPos1).xyz;

	// Normal in camera coordinates
	fN1 = normalize(cam_matrix * model_matrix * vec4(vNormal, 0)).xyz;

	// The ray from the vertex towards the camera
	fE1 = normalize(vec3(0, 0, 0) - posInCam1);

	// The ray from the vertex towards the light
	fL1 = normalize(lightInCam1.xyz - posInCam1);

	vec3 posInCam2   = (cam_matrix * model_matrix * vPosition).xyz;
	vec3 lightInCam2 = (cam_matrix * lightPos2).xyz;

	// Normal in camera coordinates
	fN2 = normalize(cam_matrix * model_matrix * vec4(vNormal, 0)).xyz;

	// The ray from the vertex towards the camera
	fE2 = normalize(vec3(0, 0, 0) - posInCam2);

	// The ray from the vertex towards the light
	fL2 = normalize(lightInCam2.xyz - posInCam2);

	gl_Position = view_matrix * model_matrix * vPosition;
}