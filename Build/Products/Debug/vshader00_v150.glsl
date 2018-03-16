#version 150

in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;
in vec2 vTexture;

uniform sampler2D textureID;

vec4 color1, color2;

uniform mat4 model_matrix;
uniform mat4 view_matrix, proj_matrix, cam_matrix;

uniform vec4  lightPos1, lightAmbient1, lightDiffuse1, lightSpecular1;
uniform vec4  lightPos2, lightAmbient2, lightDiffuse2, lightSpecular2;
uniform vec4  matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;
uniform int   enabled1, enabled2;

void main() {
    vec4 color1 = vec4(0,0,0,0);
    vec4 color2 = vec4(0,0,0,0);
    vec4 t = vec4(0,0,0,0);
	if (enabled1 == 1) {
        t = texture(textureID, vTexture);
		vec3 posInCam   = (cam_matrix * model_matrix * vPosition).xyz;
		vec3 lightInCam = (cam_matrix * lightPos1).xyz;

		// The ray from the vertex towards the camera
		vec3 V = normalize(-posInCam);
	
		// The ray fom the vertex towards the light
		vec3 L = normalize(lightInCam - posInCam);
		vec3 H = normalize(L + V);
		float dist = 1.0;

		// Normal in camera coordinates
		vec3 N = normalize(cam_matrix * model_matrix * vec4(vNormal, 0)).xyz;
	
		vec4 ambient = lightAmbient1 * matAmbient;
	
		float Kd = max(dot(L, N), 0.0);
		vec4 diffuse = Kd * lightDiffuse1 * matDiffuse;
	
		float Ks = pow(max(dot(N, H), 0.0), matAlpha);
		vec4 specular;
		if (dot(L, N) < 0.0) {
			specular = vec4(0, 0, 0, 1);
		} else {
			specular = Ks * lightSpecular1 * matSpecular;
		}

        float intensity = max(lightPos1.y / (length(L) * length(L)), 0);
		color1 = (ambient + diffuse + specular);
		color1.a = 1.0;
	}

	if (enabled2 == 1) {
        t = texture(textureID, vTexture);
		vec3 posInCam   = (cam_matrix * model_matrix * vPosition).xyz;
		vec3 lightInCam = (cam_matrix * lightPos2).xyz;

		// The ray from the vertex towards the camera
		vec3 V = normalize(-posInCam);
	
		// The ray fom the vertex towards the light
		vec3 L = normalize(lightInCam - posInCam);
		vec3 H = normalize(L + V);
		float dist = 1.0;

		// Normal in camera coordinates
		vec3 N = normalize(cam_matrix * model_matrix * vec4(vNormal, 0)).xyz;
	
		vec4 ambient = lightAmbient2 * matAmbient;
	
		float Kd = max(dot(L, N), 0.0);
		vec4 diffuse = Kd * lightDiffuse2 * matDiffuse;
	
		float Ks = pow(max(dot(N, H), 0.0), matAlpha);
		vec4 specular;
		if (dot(L, N) < 0.0) {
			specular = vec4(0, 0, 0, 1);
		} else {
			specular = Ks * lightSpecular2 * matSpecular;
		}

		float intensity = max(lightPos2.y, 0);
	
		color2 = intensity * (ambient + diffuse + specular);
		color2.a = 1.0;
	}

	color = t + color1 + color2;
	color.a = 1.0;

	gl_Position = view_matrix * model_matrix * vPosition;
}
