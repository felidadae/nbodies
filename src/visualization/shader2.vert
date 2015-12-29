#version 400

layout(location = 0) in float vertexPosition_modelspace_x;
layout(location = 1) in float vertexPosition_modelspace_y;
layout(location = 2) in float vertexPosition_modelspace_z;
uniform mat4 MVP;

void main(){
    gl_Position =  MVP * vec4(
    	vertexPosition_modelspace_x,
    	vertexPosition_modelspace_y,
    	vertexPosition_modelspace_z,
    	1);
}