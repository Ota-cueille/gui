#version 450 core

layout(location = 0) in flat uint v_slot;
layout(location = 1) in vec2 v_coords;

layout(location = 0) out vec4 color;

uniform sampler2D slots[32];

void main() {
	color = texture(slots[0], v_coords);;
}
