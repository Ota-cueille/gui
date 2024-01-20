LIBS=c:/users/viann/dev/libs/c
MINGW=c:/users/viann/dev-tools/mingw64/x86_64-w64-mingw32/lib

INCLUDES=-I include/
INCLUDES+=-I $(LIBS)/glad/include
INCLUDES+=-I $(LIBS)/glfw3/include
INCLUDES+=-I $(LIBS)/glm/include
INCLUDES+=-I $(LIBS)/stb/include
INCLUDES+=-I $(LIBS)/freetype/include

LINKS=$(LIBS)/glfw3/lib/libglfw3.a
LINKS+=$(MINGW)/libuser32.a
LINKS+=$(MINGW)/libgdi32.a

all: release debug

release: glad
	g++ -std=c++20 -c source/main.cpp -o build/release/obj/main.o $(INCLUDES)

	g++ -std=c++20 -c source/opengl/context.cpp -o build/release/obj/context.o $(INCLUDES)

	g++ -std=c++20 -c source/opengl/resources/shader.cpp -o build/release/obj/shader.o $(INCLUDES)
	g++ -std=c++20 -c source/opengl/resources/texture.cpp -o build/release/obj/texture.o $(INCLUDES)

	g++ -std=c++20 -c source/opengl/renderer.cpp -o build/release/obj/renderer.o $(INCLUDES)

	g++ -std=c++20 -o build/release/gui.exe build/release/obj/*.o $(LINKS)

debug: glad-debug
	g++ -std=c++20 -g -c source/main.cpp -o build/debug/obj/main.o $(INCLUDES)

	g++ -std=c++20 -g -c source/opengl/context.cpp -o build/debug/obj/context.o $(INCLUDES)

	g++ -std=c++20 -g -c source/opengl/resources/shader.cpp -o build/debug/obj/shader.o $(INCLUDES)
	g++ -std=c++20 -g -c source/opengl/resources/texture.cpp -o build/debug/obj/texture.o $(INCLUDES)

	g++ -std=c++20 -g -c source/opengl/renderer.cpp -o build/debug/obj/renderer.o $(INCLUDES)

	g++ -std=c++20 -g -o build/debug/gui.exe build/debug/obj/*.o $(LINKS)

glad:
	g++ -std=c++20 -c $(LIBS)/glad/src/glad.c -o build/release/obj/glad.o -I $(LIBS)/glad/include

glad-debug:
	g++ -std=c++20 -g -c $(LIBS)/glad/src/glad.c -o build/debug/obj/glad.o -I $(LIBS)/glad/include

glad-test:
	g++ -std=c++20 -g -c $(LIBS)/glad/src/glad.c -o build/test/obj/glad.o -I $(LIBS)/glad/include

test: glad-test
	g++ -std=c++20 -g -c test/test.cpp -o build/test/obj/test.o $(INCLUDES)

	g++ -std=c++20 -g -c source/opengl/context.cpp -o build/test/obj/context.o $(INCLUDES)

	g++ -std=c++20 -g -c source/opengl/resources/shader.cpp -o build/test/obj/shader.o $(INCLUDES)
	g++ -std=c++20 -g -c source/opengl/resources/texture.cpp -o build/test/obj/texture.o $(INCLUDES)

	g++ -std=c++20 -g -c source/opengl/renderer.cpp -o build/test/obj/renderer.o $(INCLUDES)

	g++ -std=c++20 -g -o build/test/test.exe build/test/obj/*.o $(LINKS)

clean:
	rm build/release/obj/*.o
	rm build/release/*.exe
	rm build/debug/obj/*.o
	rm build/debug/*.exe
	rm build/test/obj/*.o
	rm build/test/*.exe

.PHONY:
