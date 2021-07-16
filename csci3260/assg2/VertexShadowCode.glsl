#version 430

in layout(location=0) vec3 position;

uniform mat4 depthMatrix;
uniform mat4 modelTransformMatrix;

void main(){
 gl_Position = depthMatrix * modelTransformMatrix * vec4(position, 1.0);
}
