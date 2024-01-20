#version 450 core

layout(location = 0) in uint a_id;
layout(location = 1) in vec2 a_position;
layout(location = 2) in vec2 a_size;
layout(location = 3) in vec4 a_color;

layout(location = 0) out vec4 v_color;

vec2 model[] = {
	a_position,							// up    left
	a_position + vec2(0.0, a_size.y), 	// down  left
	a_position + a_size, 				// down  right
	a_position + a_size,
	a_position + vec2(a_size.x, 0.0), 	// up    right
	a_position
};

void main() {
	gl_Position = vec4(model[gl_VertexID % 6], 0.0, 1.0);
	v_color = a_color;
}
