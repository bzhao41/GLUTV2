#version 150

in vec3 fN1, fE1, fL1;
in vec3 fN2, fE2, fL2;

uniform vec4  lightAmbient1, lightDiffuse1, lightSpecular1;
uniform vec4  lightAmbient2, lightDiffuse2, lightSpecular2;
uniform vec4  matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;
uniform int   enabled1, enabled2;

vec4 fColor1, fColor2;

out vec4 fColor;

void main() { 
	if (enabled1 == 1) {
		vec3 N = normalize(fN1);
		vec3 E = normalize(fE1);
		vec3 L = normalize(fL1);
		vec3 H = normalize(L + E);

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

		fColor1 = ambient + diffuse + specular;
		fColor1.a = 1.0;
	}

	if (enabled2 == 1) {
		vec3 N = normalize(fN2);
		vec3 E = normalize(fE2);
		vec3 L = normalize(fL2);
		vec3 H = normalize(L + E);

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

		fColor2 = ambient + diffuse + specular;
		fColor2.a = 1.0;
	}
	
	fColor = fColor1 + fColor2; 
	fColor.a = 1.0;
} 