
#version 400 compatibility

uniform mat4 mvp;
out vec4 col; 
in vec4 position;
in vec3 vertexColor;
uniform float scale;

void main(){
    vec4 pos = position;
    pos.xyz = pos.xyz*scale;
	gl_Position = mvp * pos;
    col.rgb = gl_Color.rgb;
}