uniform mat4 transformMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightNormal;
uniform mat4 texMatrix;

uniform float deform;

attribute vec4 position;
attribute vec2 texCoord;

varying float interpolatedZ;
varying vec4 vertTexCoord;


void main() {
	interpolatedZ = position.z;
	gl_Position = transformMatrix * position;
	
	vertTexCoord = texMatrix * vec4(texCoord, 1.0, 1.0);
	
}
	
