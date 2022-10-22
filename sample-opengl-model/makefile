all: build
build: main.o shader.o glad.o stb_image.o camera.o mesh.o model.o
	clang++ main.o shader.o glad.o stb_image.o camera.o mesh.o model.o -o main -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp
glad.o: glad.c
	clang -I ./include -c -Wall -Wextra -Wpedantic -Werror glad.c -o glad.o
stb_image.o: stb_image.h stb_image.cpp
	clang++ -std=c++20 -I ./include -c -Wall -Wextra -Wpedantic -Werror stb_image.cpp -o stb_image.o
shader.o: shader.h shader.cpp
	clang++ -std=c++20 -I ./include -c -Wall -Wextra -Wpedantic -Werror shader.cpp -o shader.o
camera.o: camera.hpp camera.cpp
	clang++ -std=c++20 -I ./include -c -Wall -Wextra -Wpedantic -Werror camera.cpp -o camera.o
mesh.o: shader.h mesh.hpp mesh.cpp
	clang++ -std=c++20 -I ./include -c -Wall -Wextra -Wpedantic -Werror mesh.cpp -o mesh.o
model.o: mesh.hpp model.hpp model.cpp
	clang++ -std=c++20 -I ./include -c -Wall -Wextra -Wpedantic -Werror model.cpp -o model.o
main.o: model.hpp mesh.hpp camera.hpp shader.h main.cpp
	clang++ -std=c++20 -I ./include -c -Wall -Wextra -Wpedantic -Werror main.cpp -o main.o