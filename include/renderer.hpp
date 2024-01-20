#pragma once

#include "resources.hpp"
#include "primitives.hpp"

namespace renderer {

	void initialize();
	void terminate();

	void start();
	template<primitive::type Primitive> unsigned int create(typename Primitive::parameters params = {});
	template<primitive::type Primitive> void release(unsigned int id);
	template<primitive::type Primitive> Primitive* get(unsigned int id);
	template<primitive::type Primitive> void update(unsigned int id, Primitive* value);
	template<primitive::type Primitive> void draw();
	void end();

	void draw_text();

	void clear();
	void clear(float r, float g, float b, float a = 1.0f);
	void clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	void set_clear_color(float r, float g, float b, float a = 1.0f);
	void set_clear_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	void update_viewport(unsigned int width, unsigned int height);

} // namespace renderer
