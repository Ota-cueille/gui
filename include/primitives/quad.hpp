#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace primitive {

	struct quad {
		unsigned int id;
		glm::vec2 position, size;
		glm::vec4 color;

		struct parameters {
			glm::vec2 position, size;
			glm::vec4 color;
		};
	};

} // namespace renderer::primitive
