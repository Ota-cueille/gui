#include "resources/texture.hpp"

#include "platforms/opengl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/image.h>

namespace resource {

	texture texture::load(const char* filepath) {
		texture self;
		glCreateTextures(GL_TEXTURE_2D, 1, &self.id);
		glBindTexture(GL_TEXTURE_2D, self.id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		unsigned char* data = stbi_load(filepath, (int*)&self.width, (int*)&self.height, (int*)&self.channels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self.width, self.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);

		return self;
	}

} // namespace resource
