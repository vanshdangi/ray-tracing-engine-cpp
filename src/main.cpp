#include <iostream>
#include <SFML/Graphics.hpp>
#include <core/ray.hpp>
#include <rendering/camera.hpp>
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/image.hpp>
#include <core/color.hpp>
#include <rendering/renderer.hpp>
#include <world/scene.hpp>
#include <world/lighting.hpp>
#include <world/objects/sphere.hpp>
#include <world/objects/cube.hpp>
#include <world/objects/plane.hpp>
#include <memory>

int main()
{
    constexpr unsigned int WIDTH = 800;
    constexpr unsigned int HEIGHT = 600;

    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Ray Tracing Engine"
    );


    // DECLARATIONS
    Camera mainCam(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 1.0f, 0.0f),
        90.0f,
        static_cast<float>(WIDTH) / HEIGHT,
        WIDTH,
        HEIGHT
    );
    PointLight light(Point3(4.0f, 4.0f, -7.0f), 0.005f, Color(255, 255, 255));
    Image img(WIDTH, HEIGHT);
    Scene scene;

    Material mat;
    mat.albedo = Color(200, 50, 50);
    scene.addObject(std::make_unique<Sphere>(Point3(-1.0f, 5.0f, 4.0f), 1.0f, mat));
    scene.addObject(std::make_unique<Sphere>(Point3(3.0f, 7.0f, 0.0f), 1.0f, mat));
    scene.addObject(std::make_unique<AABB>(Point3(3.0f, 8.0f, 5.0f), 2.0f, mat));
    scene.addObject(std::make_unique<AABB>(Point3(2.5f, 14.0f, 4.5f), 2.0f, mat));
    scene.addObject(std::make_unique<Plane>(Point3(-2.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), mat));

    // Generate the test image.
    Renderer renderer(mainCam, img, scene, light);
    renderer.render();

    // Convert your Image into an SFML image.
    sf::Image sfImage(sf::Vector2u{WIDTH, HEIGHT});

    for (unsigned int y = 0; y < HEIGHT; ++y)
    {
        for (unsigned int x = 0; x < WIDTH; ++x)
        {
            Color color = img.getPixel(x, y);

            sfImage.setPixel(
                {x, y},
                sf::Color(
                    static_cast<std::uint8_t>(color.r),
                    static_cast<std::uint8_t>(color.g),
                    static_cast<std::uint8_t>(color.b)
                )
            );
        }
    }

    // Upload the image to the GPU.
    sf::Texture texture;
    if (!texture.loadFromImage(sfImage))
        return 1;

    sf::Sprite sprite(texture);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        window.draw(sprite);

        window.display();
    }

    return 0;
}