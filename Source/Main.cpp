#include <iostream>

#include "Logger.h"
#include "ForwardRenderer.h"

#define WIDTH    (1600)
#define HEIGHT   (1200)

// TODO: Comments
// TODO: Blurs
// TODO: Bump mapping
// TODO: Normal mapping
// TODO: Paralax mapping
// TODO: Bloom
// TODO: MRT
// TODO: Shadow map
// TODO: SSAO
// TODO: SSR
// TODO: Texture filtering
// TODO: Skybox
// TODO: Spot light
// TODO: PBR
// TODO: Simple scene graph
// TODO: Camera
// TODO: Render base class
// TODO: Deferred renderer
// TODO: Transparency
// TODO: Move math vec operators and functions into single place
// TODO: Move lighting algrorithms into Algorithms.cpp
// TODO: Gaussian blur bug duplicated central pixel
// TODO: Measure time

int main() {
    // Setup scene

    Scene scene;

    // Geometry

    //scene.LoadMeshFromObj("Data/Cube/cube.obj", false);
    //scene.LoadMeshFromObj("Data/Sponza/sponza.obj");
    //scene.LoadMeshFromObj("Data/living_room/living_room.obj");
    //scene.LoadMeshFromObj("Data/Car/Car.obj");
    //scene.LoadMeshFromObj("Data\\CornellBox\\cornellbox.obj");
    scene.LoadMeshFromObj("Data/aston-martin/source/AM-01/AM-01.obj", true, false, true);

    // Lights

    Light light;
    size_t numLights = 4;
    float_t R = 1.0f;
    for (size_t i = 0; i < numLights; i++) {
        light.lightType = LightType::Omni;
        light.position = { R * std::cosf(2 * float_t(M_PI) / numLights * i), 0.5f, R * std::sinf(2 * float_t(M_PI) / numLights * i) };
        light.ambient = { 0.1f, 0.1f, 0.1f };
        light.diffuse = { 1.0f, 1.0f, 1.0f };
        //light.diffuse = { RAND_RANGE(0.1f, 1.0f), RAND_RANGE(0.1f, 1.0f), RAND_RANGE(0.1f, 1.0f) };
        light.specular = { 1.0f, 1.0f, 1.0f };
        light.linear = 0.1f;
        light.quadratic = 0.03f;
        scene.lights.push_back(light);
    }

    light.lightType = LightType::Directional;
    light.direction = {0.0f, -1.0f, -1.0f};
    light.ambient = { 0.1f, 0.1f, 0.1f };
    light.diffuse = { 30.0f, 30.0f, 30.0f };
    light.specular = { 1.0f, 1.0f, 1.0f };
    //scene.lights.push_back(light);

    Viewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = WIDTH;
    viewport.height = HEIGHT;

    ForwardRenderer renderer{WIDTH, HEIGHT};
    Texture &result = renderer.Render(scene, viewport);

	system((result.Name() + ".bmp").c_str());

	std::cin.get();
	return 0;
}