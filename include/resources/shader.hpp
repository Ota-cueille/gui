#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace resource::shader {

	struct program;

	struct uniform {
		unsigned int location;
		const char* name;

		static uniform create(program prgm, const char* name);
	};

	struct program {
		enum shader_type : unsigned char { vertex, geometry, fragment };
		unsigned int id;

		static program create();
		static void attach(program self, const char* filepath, shader_type type);
		static void link(program self);
		static void use(program self);

		static void set(program self, uniform u, const int val);
		static void set(program self, uniform u, const glm::ivec2& val);
		static void set(program self, uniform u, const glm::ivec3& val);
		static void set(program self, uniform u, const glm::ivec4& val);

		static void set(program self, uniform u, const float val);
		static void set(program self, uniform u, const glm::vec2& val);
		static void set(program self, uniform u, const glm::vec3& val);
		static void set(program self, uniform u, const glm::vec4& val);

		static void set(program self, uniform u, const unsigned int* val, unsigned int count);
		static void set(program self, uniform u, const int* val, unsigned int count);
	};

} // namespace resource::shader
