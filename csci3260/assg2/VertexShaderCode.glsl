#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec2 uv;
in layout(location=2) vec3 normal;

uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 depthBiasMatrix;
uniform mat4 projectionMatrix;

out vec2 UV;
out vec3 normalPos;
out vec3 fragPos;
out vec4 shadowPos;

///////////////////////////////////////////////////////////////

void main() {
	vec4 pos = vec4(position, 1.0);
	vec4 newPosition = viewMatrix * modelTransformMatrix * pos;
	gl_Position = projectionMatrix * newPosition;

	vec4 nrm = modelTransformMatrix * vec4(normal, 0.0);
	normalPos = vec3(nrm);

	fragPos = vec3(newPosition);
	shadowPos = depthBiasMatrix * modelTransformMatrix * pos;
	UV = uv;
}
