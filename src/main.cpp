#include <iostream>
#include <memory>
#include <charconv>
#include <string>
#include <system_error>
#include <SFML/Graphics.hpp>
#include <render_stats.hpp>
#include <core/image.hpp>
#include <rendering/renderer.hpp>
#include <scenes/demoScenes.hpp>
#include <world/objects/mesh.hpp>

unsigned int parsePositiveInt(const std::string& value, const std::string& option);

int main(int argc, char* argv[])
{
    unsigned int WIDTH = 1440;
    unsigned int HEIGHT = 810;

    std::string sceneName = "benchmark";
    std::string pngName = "render.png";
    bool useBVH = true;
    bool headLess = false;
    std::size_t samplesPerPixel = 16;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--scene") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --scene requires a scene name\n";
                return 1;
            }

            sceneName = argv[++i];
        } else if(arg == "--output") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --output requires a png name\n";
                return 1;
            }

            pngName = argv[++i];
        } else if (arg == "--spp") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --spp requires a positive integer\n";
                return 1;
            }

            const std::string value = argv[++i];
            const auto result = std::from_chars(
                value.data(), value.data() + value.size(), samplesPerPixel);
            if (result.ec != std::errc{} ||
                result.ptr != value.data() + value.size() ||
                samplesPerPixel == 0) {
                std::cerr << "Error: --spp must be a positive integer\n";
                return 1;
            }
        } else if (arg == "--width" || arg == "--height") {
            if (i + 1 >= argc) {
                std::cerr << "Error: " << arg
                        << " requires a positive integer\n";
                return 1;
            }

            const unsigned int value =
                parsePositiveInt(argv[++i], arg);

            if (arg == "--width") {
                WIDTH = value;
            } else {
                HEIGHT = value;
            }
        } else if (arg == "--no-bvh") {
            useBVH = false;
        } else if (arg == "--headless") {
            headLess = true;
        } else {
            std::cerr << "Error: Unknown argument: " << arg << '\n';
            return 1;
        }
    }

    Mesh::setBVHEnabled(useBVH);

    SceneConfig config = [&]() {
        if (sceneName == "bunny")
            return createBunnyScene(WIDTH, HEIGHT);

        if (sceneName == "reflections")
            return createReflectionsScene(WIDTH, HEIGHT);

        if (sceneName == "benchmark")
            return createBenchmarkScene(WIDTH, HEIGHT);

        throw std::invalid_argument("Unknown scene: " + sceneName);
    }();

    Image img(WIDTH, HEIGHT);
    Renderer renderer(config.camera, img, config.scene, config.light, config.ambientIntensity);
    renderer.setSamplesPerPixel(samplesPerPixel);
    renderer.render();

    const RenderStats::Snapshot stats = renderStats.snapshot();
    std::cout << "Render time: " << stats.renderTimeMs << " ms\n";
    std::cout << "Pixels rendered: " << stats.pixelsRendered << '\n';
    std::cout << "Primary rays: " << stats.primaryRays << '\n';
    std::cout << "Rays traced: " << stats.raysTraced << '\n';
    std::cout << "Shadow rays: " << stats.shadowRays << '\n';
    std::cout << "Triangle tests: " << stats.triangleTests << '\n';
    std::cout << "Pixels per second: " << stats.pixelsPerSecond << '\n';
    std::cout << "Rays per second: " << stats.raysPerSecond << '\n';
    std::cout << "Triangle tests per primary ray: "
              << stats.triangleTestsPerPrimaryRay << '\n';

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
                    toSrgbByte(color.r),
                    toSrgbByte(color.g),
                    toSrgbByte(color.b)
                )
            );
        }
    }

    // Save Image
    if (!sfImage.saveToFile(pngName)) {
        std::cerr << "Error: Failed to save image to "
                << pngName << '\n';
        return 1;
    }

    if (!headLess) {
        sf::RenderWindow window(
            sf::VideoMode({WIDTH, HEIGHT}),
            "Ray Tracing Engine"
        );

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
    }

    return 0;
}


unsigned int parsePositiveInt(const std::string& value, const std::string& option) {
    unsigned int result = 0;

    const auto [ptr, ec] = std::from_chars(
        value.data(), value.data() + value.size(), result
    );

    if (ec != std::errc{} ||
        ptr != value.data() + value.size() ||
        result == 0) {
        throw std::invalid_argument(
            option + " must be a positive integer"
        );
    }

    return result;
}