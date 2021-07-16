#version 430

out layout(location=0) float depth;

void main() {
	depth = gl_FragCoord.z;
}