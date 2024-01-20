#pragma once

#include <glm/vec2.hpp>
#include "resources/texture.hpp"

namespace primitive {

	struct texture {
		unsigned int id, slot;
		glm::vec2 position, size;

		struct parameters {
			const resource::texture& texture;
			glm::vec2 position, size;
		};
	};

} // namespace renderer::primitive
