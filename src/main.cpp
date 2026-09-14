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
#include <world/objects/aabb.hpp>
#include <world/objects/plane.hpp>
#include <world/objects/triangle.hpp>
#include <world/objects/mesh.hpp>
#include <parser/objParser.hpp>
#include <memory>

Scene createDemoScene() {
    Scene scene;
    
    // Materials
    Material red;
    red.albedo = Color(200, 50, 50);
    
    Material green;
    green.albedo = Color(50, 200, 80);
    
    Material blue;
    blue.albedo = Color(50, 100, 220);
    
    Material yellow;
    yellow.albedo = Color(220, 200, 50);
    
    Material monkeyMat;
    monkeyMat.albedo = Color(180, 100, 50);
    
    
    // --------------------------------------------------
    // Sphere
    // --------------------------------------------------
    
    Transform sphereTransform(
        Point3(-3.0f, 0.0f, 8.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.5f, 1.5f, 1.5f)
    );

    scene.addObject(
        std::make_unique<Sphere>(
            sphereTransform,
            1.0f,
            red
        )
    );
    
    
    // --------------------------------------------------
    // AABB
    // --------------------------------------------------
    
    Transform boxTransform(
        Point3(3.0f, 0.0f, 8.0f),
        Vec3(0.0f, 0.0f, 30.0f),
        Vec3(1.5f, 1.0f, 1.0f)
    );
    
    scene.addObject(
        std::make_unique<AABB>(
            boxTransform,
            1.0f,
            green
        )
    );
    
    
    // --------------------------------------------------
    // Plane
    // --------------------------------------------------
    
    Transform planeTransform(
        Point3(0.0f, -2.0f, 8.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
    
    scene.addObject(
        std::make_unique<Plane>(
            planeTransform,
            Vec3(0.0f, 1.0f, 0.0f),
            blue
        )
    );
    
    
    // --------------------------------------------------
    // Standalone triangle
    // --------------------------------------------------
    
    scene.addObject(
        std::make_unique<Triangle>(
            Point3(-1.0f, 1.5f, 6.0f),
            Point3( 0.0f, 3.5f, 6.0f),
            Point3( 1.0f, 1.5f, 6.0f),
            yellow
        )
    );
    
    
    // --------------------------------------------------
    // Mesh
    // --------------------------------------------------
    
    OBJParser parser;
    
    OBJData data = parser.parser("objects/suzanne.obj");
    
    Transform meshTransform(
        Point3(0.0f, 0.0f, 10.0f),
        Vec3(0.0f, 180.0f, 0.0f),
        Vec3(2.0f, 2.0f, 2.0f)
    );
    
    scene.addObject(
        std::make_unique<Mesh>(
            meshTransform,
            data,
            monkeyMat
        )
    );

    
    return scene;
    
}


int main()
{
    constexpr unsigned int WIDTH = 1440;
    constexpr unsigned int HEIGHT = 810;

    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Ray Tracing Engine"
    );

    // DECLARATIONS
    Camera mainCam(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 1.0f),
        90.0f,
        static_cast<float>(WIDTH) / HEIGHT,
        WIDTH,
        HEIGHT
    );
    PointLight light(
        Point3(0.0f, 5.0f, 2.0f),
        0.005f,
        Color(255, 255, 255)
    );

    Image img(WIDTH, HEIGHT);
    Scene scene = createDemoScene();
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
