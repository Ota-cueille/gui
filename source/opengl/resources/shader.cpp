#include "resources/shader.hpp"

#include <cstdio>
#include <glad/glad.h>

using namespace resource::shader;

static unsigned int compile_shader(const char* path, unsigned int type) {
	char _[4096] = { '\0' }; char* buf = _;
	FILE* shader_source_file = fopen(path, "r");
	fread(buf, sizeof(char), 4096, shader_source_file);
	fclose(shader_source_file);

	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &buf, nullptr);
	glCompileShader(id);
	return id;
}

uniform uniform::create(program prgm, const char* name) {
	uniform self;
	self.name = name;
	int loc = glGetUniformLocation(prgm.id, name);
	if(loc < 0) {
		self.location = 0;
		printf("error %s uniform doesn't exists in program");
	}
	self.location = (unsigned int)loc;
	return self;
}

program program::create() {
	program self;
	self.id = glCreateProgram();
	return self;
}

void program::attach(program self, const char* filepath, program::shader_type type) {
	unsigned int shader_id;
	switch(type) {
		case shader_type::vertex: shader_id = glCreateShader(GL_VERTEX_SHADER); break;
		case shader_type::geometry: shader_id = glCreateShader(GL_GEOMETRY_SHADER); break;
		case shader_type::fragment: shader_id = glCreateShader(GL_FRAGMENT_SHADER); break;
		default: printf("Error unknown shader type\n"); break;
	}

	char _[4096] = { '\0' }; char* buf = _;
	FILE* shader_source_file = fopen(filepath, "r");
	fread(buf, sizeof(char), 4096, shader_source_file);
	fclose(shader_source_file);

	glShaderSource(shader_id, 1, &buf, nullptr);
	glCompileShader(shader_id);
	glAttachShader(self.id, shader_id);
}

void program::link(program self) {
	glLinkProgram(self.id);
}

void program::use(program self) {
	glUseProgram(self.id);
}

void program::set(program self, uniform u, const int val) {
	glUniform1i(u.location, val);
}

void program::set(program self, uniform u, const glm::ivec2& val) {
	glUniform2i(u.location, val.x, val.y);
}

void program::set(program self, uniform u, const glm::ivec3& val) {
	glUniform3i(u.location, val.x, val.y, val.z);
}

void program::set(program self, uniform u, const glm::ivec4& val) {
	glUniform4i(u.location, val.x, val.y, val.z, val.w);
}

void program::set(program self, uniform u, const float val) {
	glUniform1f(u.location, val);
}

void program::set(program self, uniform u, const glm::vec2& val) {
	glUniform2f(u.location, val.x, val.y);
}

void program::set(program self, uniform u, const glm::vec3& val) {
	glUniform3f(u.location, val.x, val.y, val.z);
}

void program::set(program self, uniform u, const glm::vec4& val) {
	glUniform4f(u.location, val.x, val.y, val.z, val.w);
}

void program::set(program self, uniform u, const int* val, unsigned int count) {
	glUniform1iv(u.location, count, val);
}

void program::set(program self, uniform u, const unsigned int* val, unsigned int count) {
	glUniform1uiv(u.location, count, val);
}
