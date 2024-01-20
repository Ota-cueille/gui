#include <cstdio>
#include <cstdlib>

#include <gui.hpp>

inline constexpr unsigned char operator ""_u8(unsigned long long arg) noexcept { return static_cast<unsigned char>(arg); }

namespace ctx = context;
namespace gfx = renderer;
namespace res = resource;

int main(int ac, char* av[]) {
	ctx::initialize();
	gfx::initialize();
	gfx::set_clear_color(42_u8, 46_u8, 75_u8);

	res::texture block_tex = res::texture::load("./assets/textures/brick block.png");
	unsigned int block = gfx::create<primitive::texture>(primitive::texture::parameters{
		.texture = block_tex,
		.position = glm::vec2(-1.0f, 0.0f), .size = glm::vec2(1.0f, -1.0f)
	});

	gfx::create<primitive::quad>(primitive::quad::parameters{
		.position = glm::vec2(0.0f, 0.0f), .size = glm::vec2(1.0f, 1.0f),
		.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
	});

	gfx::create<primitive::bezier>(primitive::bezier::parameters{
		.start = glm::vec2(-1.0f, 0.0f), .end = glm::vec2(1.0f, 0.0f),
		.control1 = glm::vec2(-0.5f, 2.0f), .control2 = glm::vec2(0.5f, -2.0f)
	});

	while(ctx::running) {
		if(ctx::redraw) {
			gfx::start();

			gfx::clear();
			gfx::draw<primitive::texture>();
			gfx::draw<primitive::quad>();
			gfx::draw<primitive::bezier>();

			gfx::end();

			ctx::swap_buffers();
			ctx::redraw = false;
		}

		ctx::poll_events();
	}

	gfx::terminate();
	ctx::terminate();
	return 0;
}
