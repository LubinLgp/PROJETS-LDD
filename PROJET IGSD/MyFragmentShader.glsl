#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform float fraction;
uniform sampler2D texture;

varying float interpolatedZ;
varying vec4 vertTexCoord;

void main() {
	float remainder = mod(interpolatedZ, 2.0);
	if(interpolatedZ<=-199){
		gl_FragColor = vec4(0.106, 0.31, 0.031, 1.0)*0.5+texture2D(texture, vertTexCoord.st)*0.5;
	}else if (remainder<=0.15) {
        	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
   	} else {
		gl_FragColor = texture2D(texture, vertTexCoord.st);
	}

}


