#pragma once

namespace context {

	inline bool redraw = true;
	inline bool running = true;

	inline unsigned int width = 1080, height = 720;

	namespace mouse {

		inline double x, y;

	} // namespace mouse

	void initialize();
	void terminate();

	void poll_events();
	void swap_buffers();

} // namespace context
