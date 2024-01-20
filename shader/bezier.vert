#version 450 core

layout(location = 0) in uint a_id;
layout(location = 1) in vec2 a_start;
layout(location = 2) in vec2 a_end;
layout(location = 3) in vec2 a_control1;
layout(location = 4) in vec2 a_control2;

const int VertexPerInstance = 100-1; // keep this in sinc with the draw call int the bezier implementation
const float Step = 1.0/float(VertexPerInstance);

void main() {
	// calculate with gl_VertexID the current point in the curve we are at
	const float t = gl_VertexID * Step;
	gl_Position = vec4(a_end * t*t*t , 0.0, 0.25) 
                    + vec4(a_control2 * 3*t*t*(1-t)      , 0.0, 0.25) 
                    + vec4(a_control1 * 3*t*  (1-t)*(1-t), 0.0, 0.25) 
                    + vec4(a_start    * (1-t)*(1-t)*(1-t), 0.0, 0.25);
}
