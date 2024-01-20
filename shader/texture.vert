#version 450 core

layout(location = 0) in uint a_id;
layout(location = 1) in uint a_slot;
layout(location = 2) in vec2 a_position;
layout(location = 3) in vec2 a_size;

layout(location = 0) out flat uint v_slot;
layout(location = 1) out vec2 v_texture_coordinate;

const vec2 model[] = {
	a_position,							// up    left
	a_position + vec2(0.0, a_size.y), 	// down  left
	a_position + a_size, 				// down  right
	a_position + a_size,
	a_position + vec2(a_size.x, 0.0), 	// up    right
    a_position
};

const vec2 texture_coordinate_model[] = {
    vec2(0.0, 1.0), // up    left
    vec2(0.0, 0.0), // down  left
    vec2(1.0, 0.0), // down  right
    vec2(1.0, 0.0),
    vec2(1.0, 1.0), // up    right
    vec2(0.0, 1.0)
};

void main() {
    gl_Position = vec4(model[gl_VertexID % 6], 0.0, 1.0);
    v_slot = a_slot;
    v_texture_coordinate = texture_coordinate_model[gl_VertexID % 6];
}
