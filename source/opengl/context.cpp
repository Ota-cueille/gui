#include "context.hpp"

#define internal static

#include <cstdio>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"

// Callbacks
internal void on_window_resize(GLFWwindow* window, int width, int height);
internal void on_window_close(GLFWwindow* window);
internal void on_window_refresh(GLFWwindow* window);

internal void on_mouse_moved(GLFWwindow* window, double x, double y);
internal void on_mouse_clicked(GLFWwindow* window, int button, int action, int mods);

namespace context {

	internal GLFWwindow* window;

	void initialize() {
		if(!glfwInit()) {
			printf("glfwInit() error !\n");
			exit(-1);
		}

		window = glfwCreateWindow(width, height, "gui", nullptr, nullptr);
		glfwMakeContextCurrent(window);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		// initial state
		glfwGetCursorPos(window, &mouse::x, &mouse::y);

		glfwSwapInterval(1);
		glfwSetFramebufferSizeCallback(window, on_window_resize);
		glfwSetWindowCloseCallback(window, on_window_close);
		glfwSetCursorPosCallback(window, on_mouse_moved);
		glfwSetMouseButtonCallback(window, on_mouse_clicked);
		glfwSetWindowRefreshCallback(window, on_window_refresh);
	}

	void terminate() { glfwDestroyWindow(window); glfwTerminate(); }

	void swap_buffers() { glfwSwapBuffers(window); }

	void poll_events() { glfwPollEvents(); }

} // namespace context

void on_window_resize(GLFWwindow* window, int width, int height) {
	context::width = width;
	context::height = height;
	renderer::update_viewport((unsigned int)(width), (unsigned int)(height));
}

void on_window_close(GLFWwindow* window) {
	context::running = false;
}

void on_window_refresh(GLFWwindow* window) {
	context::redraw = true;
}

void on_mouse_moved(GLFWwindow* window, double x, double y) {
	// useful for interactive elements ?
	// context::redraw = true;

	context::mouse::x = x;
	context::mouse::y = y;
}

void on_mouse_clicked(GLFWwindow* window, int button, int action, int mods) {

}
