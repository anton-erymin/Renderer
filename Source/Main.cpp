#include <iostream>

#include "Logger.h"
#include "Render.h"

int main() {
	Texture screen{ "Screen", 1024, 768, 3, sizeof(float_t) };
    //Mesh mesh{"Data\\Cube\\cube.obj"};
    Mesh mesh{"Data\\Car\\Car.obj"};
    LOG(mesh);

    Render render;

    render.RenderMesh(mesh, PolygonMode::Line, screen);

    auto screenFile = screen.Name() + ".bmp";
	screen.SaveBmp(screenFile);
	system(screenFile.c_str());

	std::cin.get();
	return 0;
}