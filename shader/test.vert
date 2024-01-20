#version 450 core

layout(location = 0) in uint a_slot;
layout(location = 1) in vec2 a_position;
layout(location = 2) in vec2 a_size;

layout(location = 0) out flat uint v_slot;
layout(location = 1) out vec2 v_coords;

vec2 position_model[] = {
	a_position,
	a_position + vec2(0.0, a_size.y),
	a_position + a_size,

	a_position + a_size,
	a_position + vec2(a_size.x, 0.0),
	a_position
};

vec2 coords_model[] = {
	vec2(0.0, 1.0),
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),

	vec2(1.0, 0.0),
	vec2(1.0, 1.0),
	vec2(0.0, 1.0),
};

void main() {
	gl_Position = vec4(position_model[gl_VertexID % 6], 0.0, 1.0);
	v_slot = a_slot;
	v_coords = coords_model[gl_VertexID % 6];
}
