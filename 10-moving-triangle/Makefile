GCC = gcc -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++11 -Wall -g -lm -lstdc++ -lGL -lGLEW -lglfw -lX11 -lpthread -lXrandr -lXi

main: main.cpp shader.o vertexShader.glsl fragmentShader.glsl
	${GCC} -o $@ main.cpp shader.o

shader.o: shader.cpp shader.hpp
	${GCC} -c -o $@ shader.cpp

clean:
	rm -f main shader.o

.PHONY: clean
