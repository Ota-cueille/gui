#include "platforms/opengl.hpp"

#include "renderer.hpp"
#include "resources.hpp"
#include "primitives.hpp"

#include <vector>

#include <ft2build.h>
#include <freetype/freetype.h>

namespace renderer {

    constexpr unsigned char max_quad = 10;
	constexpr unsigned char max_texture_slots = 32;

	struct texture_batch { unsigned int ids[32], start, count; };

	struct Self {
		bool updating = false;

		struct {
			unsigned int count = 0;
			primitive::quad* instances;
			resource::shader::program program;

			unsigned int vao, vbo;
			unsigned long long offsets[max_quad];
		} quad;

		struct {
			unsigned int count = 0;
			primitive::texture* instances;
			resource::shader::program program;

			unsigned int vao, vbo;
			unsigned long long offsets[max_quad];

			unsigned char current_batch_size = 0;
			std::vector<texture_batch> batches;
		} texture;

		struct {
			unsigned int count = 0;
			primitive::bezier* instances;
			resource::shader::program program;

			unsigned int vao, vbo;
			unsigned long long offsets[max_quad];
		} bezier;
	};

	inline Self self;

} // namespace renderer

#include "context.hpp"

#include <cstdio>
#include <cstring>

// Callbacks
void gl_error(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char *message, const void*);

namespace renderer {

	void initialize() {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC)gl_error, nullptr);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		/* QUAD INSTANCE DATA */
		memset(self.quad.offsets, 0xFF, sizeof(self.quad.offsets));

		self.quad.program = resource::shader::program::create();
		resource::shader::program::attach(self.quad.program, "./shader/quad.vert", resource::shader::program::shader_type::vertex);
		resource::shader::program::attach(self.quad.program, "./shader/quad.frag", resource::shader::program::shader_type::fragment);
		resource::shader::program::link(self.quad.program);

		glCreateVertexArrays(1, &self.quad.vao);
		glBindVertexArray(self.quad.vao);

		glCreateBuffers(1, &self.quad.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, self.quad.vbo);
		glBufferData(GL_ARRAY_BUFFER, max_quad * sizeof(primitive::quad), nullptr, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(primitive::quad), (const void*)offsetof(primitive::quad, id));
		glVertexAttribDivisor(0, 1);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(primitive::quad), (const void*)offsetof(primitive::quad, position));
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(primitive::quad), (const void*)offsetof(primitive::quad, size));
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(primitive::quad), (const void*)offsetof(primitive::quad, color));
		glVertexAttribDivisor(3, 1);

		glBindVertexArray(0);

		/* TEXTURE INSTANCE DATA */
		memset(self.texture.offsets, 0xFF, sizeof(self.texture.offsets));

		self.texture.program = resource::shader::program::create();
		resource::shader::program::attach(self.texture.program, "./shader/texture.vert", resource::shader::program::shader_type::vertex);
		resource::shader::program::attach(self.texture.program, "./shader/texture.frag", resource::shader::program::shader_type::fragment);
		resource::shader::program::link(self.texture.program);

		glCreateVertexArrays(1, &self.texture.vao);
		glBindVertexArray(self.texture.vao);

		glCreateBuffers(1, &self.texture.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, self.texture.vbo);
		glBufferData(GL_ARRAY_BUFFER, max_quad * sizeof(primitive::texture), nullptr, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(primitive::texture), (const void*)offsetof(primitive::texture, id));
		glVertexAttribDivisor(0, 1);

		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(primitive::texture), (const void*)offsetof(primitive::texture, slot));
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(primitive::texture), (const void*)offsetof(primitive::texture, position));
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(primitive::texture), (const void*)offsetof(primitive::texture, size));
		glVertexAttribDivisor(3, 1);

		// INIT batches
		self.texture.batches.push_back({ .ids = {0}, .start = 0, .count = 0 });

		glBindVertexArray(0);

		/* BEZIER INSTANCE DATA */
		memset(self.bezier.offsets, 0xFF, sizeof(self.bezier.offsets));

		self.bezier.program = resource::shader::program::create();
		resource::shader::program::attach(self.bezier.program, "./shader/bezier.vert", resource::shader::program::shader_type::vertex);
		resource::shader::program::attach(self.bezier.program, "./shader/bezier.frag", resource::shader::program::shader_type::fragment);
		resource::shader::program::link(self.bezier.program);

		glCreateVertexArrays(1, &self.bezier.vao);
		glBindVertexArray(self.bezier.vao);

		glCreateBuffers(1, &self.bezier.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, self.bezier.vbo);
		glBufferData(GL_ARRAY_BUFFER, max_quad * sizeof(primitive::bezier), nullptr, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(primitive::bezier), (const void*)offsetof(primitive::bezier, id));
		glVertexAttribDivisor(0, 1);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(primitive::bezier), (const void*)offsetof(primitive::bezier, primitive::bezier::start));
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(primitive::bezier), (const void*)offsetof(primitive::bezier, primitive::bezier::end));
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(primitive::bezier), (const void*)offsetof(primitive::bezier, control1));
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, false, sizeof(primitive::bezier), (const void*)offsetof(primitive::bezier, control2));
		glVertexAttribDivisor(4, 1);

		glBindVertexArray(0);

		// end rendering session <=> start updating session
		renderer::end();
	}

	void terminate() {}

	// setup buffers for rendering
	void start() {
		glUnmapNamedBuffer(self.quad.vbo);
		glUnmapNamedBuffer(self.texture.vbo);
		glUnmapNamedBuffer(self.bezier.vbo);
		self.updating = false;

		// update projection matrix in all shaders
		// self.quad.program.uniform("u_projection", glm::ortho(0.0f, context::width, context::height, 0.0f, -1.0f, 0.0f));
	}

	// enable application to update renderer's data
	void end() {
		self.quad.instances = (primitive::quad*)glMapNamedBuffer(self.quad.vbo, GL_WRITE_ONLY);
		self.texture.instances = (primitive::texture*)glMapNamedBuffer(self.texture.vbo, GL_WRITE_ONLY);
		self.bezier.instances = (primitive::bezier*)glMapNamedBuffer(self.bezier.vbo, GL_WRITE_ONLY);
		self.updating = true;
	}

	template<>
	unsigned int create<primitive::quad>(primitive::quad::parameters params) {
		assert(self.updating && "Assertion Failed : cannot create quad instance while rendering !");
		assert(self.quad.count < max_quad && "Assertion Failed: Too many quad instanciated !");

		// find an available block to store the ID
		unsigned int available_block;
		for(
			available_block = 0;

			available_block  < max_quad &&
			self.quad.offsets[available_block] != ((unsigned long long)(-1));

			available_block++
		);

		unsigned int id = self.quad.count++;
		self.quad.offsets[available_block] = id;
		primitive::quad* q = &self.quad.instances[id];
		q->id = id;
		q->position = params.position;
		q->size = params.size;
		q->color = params.color;

		return available_block;
	}

	template<>
	void release<primitive::quad>(unsigned int id) {
		assert(self.updating && "Assertion Failed : cannot release quad instance while rendering !");
		// we must remove quad inside the buffer
		// so we move the last on to override it
		// and decrement the count of instances
		unsigned int offset = self.quad.offsets[id];
		if(offset != self.quad.count-1) {
			memcpy(&self.quad.instances[offset], &self.quad.instances[self.quad.count-1], sizeof(primitive::quad));
			self.quad.offsets[id] = (unsigned int)(-1);
		}
		self.quad.count--;
	}

	template<>
	primitive::quad* get(unsigned int id) {
		assert(self.updating && "Assertion Failed: You must end the rendering before accessing renderer data !");
		assert(self.quad.offsets[id] != ((unsigned int)(-1)) && "Assertion Failed: Provided ID is not valid !");

		return &self.quad.instances[self.quad.offsets[id]];
	}

	template<>
	void update(unsigned int id, primitive::quad* value) {
		assert(value && "Assertion Failed: Provided value is ill formed !");
		assert(self.updating && "Assertion Failed: You must end the rendering before updating renderer data !");
		assert(self.quad.offsets[id] != ((unsigned int)(-1)) && "Assertion Failed: Provided ID is not valid !");

		memcpy(&self.quad.instances[self.quad.offsets[id]], value, sizeof(primitive::quad));
	}

	template<>
	void draw<primitive::quad>() {
		resource::shader::program::use(self.quad.program);
		glBindVertexArray(self.quad.vao);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, self.quad.count);
	}

	template<>
	unsigned int create<primitive::texture>(primitive::texture::parameters params) {
		assert(self.updating && "Assertion Failed : cannot create texture instance while rendering !");
		assert(self.texture.count < max_quad && "Assertion Failed: Too many texture instanciated !");

		// find an available block to store the ID
		unsigned int available_block;
		for(
			available_block = 0;

			available_block < max_quad &&
			self.texture.offsets[available_block] != ((unsigned long long)(-1));

			available_block++
		);

		unsigned int id = self.texture.count++;
		self.texture.offsets[available_block] = id;
		self.texture.instances[self.texture.offsets[available_block]].id = id;
		self.texture.instances[self.texture.offsets[available_block]].position = params.position;
		self.texture.instances[self.texture.offsets[available_block]].size = params.size;

		texture_batch& batch = self.texture.batches.back();
		if(self.texture.current_batch_size >= 32) {
			self.texture.current_batch_size = 0;
			// attention en fait on devrait peut être le faire avant de créer l'élément dans le buffer `self.texture.count`
			batch.count = self.texture.count - batch.start;
			self.texture.batches.push_back({ .ids = {0}, .start = self.texture.count });
			batch = self.texture.batches.back();
		}

		unsigned int found;
		for(found = 0; found < self.texture.current_batch_size && batch.ids[found] != params.texture.id; found++);
		if(found == self.texture.current_batch_size) {
			self.texture.current_batch_size++;
			batch.ids[found] = params.texture.id;
			self.texture.instances[self.texture.offsets[available_block]].slot = found;
		}
		batch.count++;

		// find available texture batch
		return available_block;
	}

	template<>
	void release<primitive::texture>(unsigned int id) {
		assert(self.updating && "Assertion Failed : cannot release texture instance while rendering !");
		// we must remove texture inside the buffer
		// so we move the last on to override it
		// and decrement the count of instances
		unsigned int offset = self.texture.offsets[id];
		if(offset != self.texture.count-1) {
			memcpy(&self.texture.instances[offset], &self.texture.instances[self.texture.count-1], sizeof(primitive::texture));
			self.texture.offsets[id] = (unsigned int)(-1);
		}
		self.texture.count--;
	}

	template<>
	primitive::texture* get(unsigned int id) {
		assert(self.updating && "Assertion Failed: You must end the rendering before accessing renderer data !");
		assert(self.texture.offsets[id] != ((unsigned int)(-1)) && "Assertion Failed: Provided ID is not valid !");

		return &self.texture.instances[self.texture.offsets[id]];
	}

	template<>
	void update(unsigned int id, primitive::texture* value) {
		assert(value && "Assertion Failed: Provided value is ill formed !");
		assert(self.updating && "Assertion Failed: You must end the rendering before updating renderer data !");
		assert(self.texture.offsets[id] != ((unsigned int)(-1)) && "Assertion Failed: Provided ID is not valid !");

		memcpy(&self.texture.instances[self.texture.offsets[id]], value, sizeof(primitive::bezier));
	}

	template<>
	void draw<primitive::texture>() {
		resource::shader::program::use(self.texture.program);
		glBindVertexArray(self.texture.vao);

		// iterate threw all texture slots and bind them
		for(auto batch : self.texture.batches) {
			for(unsigned int i = 0; i < max_texture_slots; i++) {
				glBindTextureUnit(i, batch.ids[i]);
			}
			glDrawArraysInstancedBaseInstance(GL_TRIANGLE_STRIP, 0, 6, batch.count, batch.start);
		}
	}

	template<>
	unsigned int create<primitive::bezier>(primitive::bezier::parameters params) {
		assert(self.updating && "Assertion Failed : cannot create bezier instance while rendering !");
		assert(self.bezier.count < max_quad && "Assertion Failed: Too many bezier instanciated !");

		// find an available block to store the ID
		unsigned int available_block;
		for(
			available_block = 0;

			available_block  < max_quad &&
			self.bezier.offsets[available_block] != ((unsigned long long)(-1));

			available_block++
		);

		unsigned int id = self.bezier.count++;
		self.bezier.offsets[available_block] = id;
		primitive::bezier* b = &self.bezier.instances[id];
		b->id = id;
		b->start = params.start;
		b->end = params.end;
		b->control1 = params.control1;
		b->control2 = params.control2;

		return available_block;
	}

	template<>
	void release<primitive::bezier>(unsigned int id) {
		assert(self.updating && "Assertion Failed : cannot release texture instance while rendering !");
		// we must remove texture inside the buffer
		// so we move the last on to override it
		// and decrement the count of instances
		unsigned int offset = self.bezier.offsets[id];
		if(offset != self.bezier.count-1) {
			memcpy(&self.bezier.instances[offset], &self.bezier.instances[self.bezier.count-1], sizeof(primitive::bezier));
			self.bezier.offsets[id] = (unsigned int)(-1);
		}
		self.bezier.count--;
	}

	template<>
	primitive::bezier* get(unsigned int id) {
		assert(self.updating && "Assertion Failed: You must end the rendering before accessing renderer data !");
		assert(self.bezier.offsets[id] != ((unsigned int)(-1)) && "Assertion Failed: Provided ID is not valid !");

		return &self.bezier.instances[self.bezier.offsets[id]];
	}

	template<>
	void update(unsigned int id, primitive::bezier* value) {
		assert(value && "Assertion Failed: Provided value is ill formed !");
		assert(self.updating && "Assertion Failed: You must end the rendering before updating renderer data !");
		assert(self.bezier.offsets[id] != ((unsigned int)(-1)) && "Assertion Failed: Provided ID is not valid !");

		memcpy(&self.bezier.instances[self.bezier.offsets[id]], value, sizeof(primitive::bezier));
	}

	template<>
	void draw<primitive::bezier>() {
		resource::shader::program::use(self.bezier.program);
		glBindVertexArray(self.bezier.vao);

		glDrawArraysInstanced(GL_LINE_STRIP, 0, 100, self.bezier.count);
	}

	// void draw_text() {
	// 	glBindVertexArray(self.text.vao);
	// 	glBindTextureUnit(0, self.text.font_texture);
	// 	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
	// }

	void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void clear(float r, float g, float b, float a) { glClearColor(r, g, b, a); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { glClearColor(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	void set_clear_color(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
	void set_clear_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { glClearColor(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f); }
	void update_viewport(unsigned int width, unsigned int height) { glViewport(0, 0, width, height); }

} // namespace renderer

void gl_error(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char *message, const void*) {
    const char* source_name;
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             source_name = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_name = "window system"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: source_name = "shader compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     source_name = "third party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     source_name = "application"; break;
        case GL_DEBUG_SOURCE_OTHER:           source_name = "other"; break;
    }
    const char* type_name;
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               type_name = "error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_name = "deprecated behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_name = "undefined behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         type_name = "portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         type_name = "performance"; break;
        case GL_DEBUG_TYPE_MARKER:              type_name = "marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          type_name = "push group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           type_name = "pop group"; break;
        case GL_DEBUG_TYPE_OTHER:               type_name = "other"; break;
    }
    const char* severity_name;
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         severity_name = "high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severity_name = "medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          severity_name = "low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severity_name = "notification"; break;
    }
    printf("GL ERROR[source: %s, type: %s, severity: %s]: %s\n", source_name, type_name, severity_name, message);
}
