#include <scenes/demoScenes.hpp>
#include <core/ray.hpp>
#include <maths/point3.hpp>
#include <maths/vec3.hpp>
#include <core/image.hpp>
#include <core/color.hpp>
#include <rendering/renderer.hpp>
#include <world/scene.hpp>
#include <world/objects/sphere.hpp>
#include <world/objects/aabb.hpp>
#include <world/objects/plane.hpp>
#include <world/objects/triangle.hpp>
#include <world/objects/mesh.hpp>
#include <parser/objParser.hpp>
#include <core/colorUtils.hpp>

SceneConfig createBenchmarkScene(
    unsigned int width,
    unsigned int height
) {
    Scene scene;
    
    // Materials
    Material red;
    red.albedo = Color::fromSrgb(0.784f, 0.196f, 0.196f);

    Material white;
    white.albedo = Color(1.0f, 1.0f, 1.0f);
    
    // Suzanne
    OBJData data = loadOBJ("objects/suzanne.obj");
    Transform meshTransform(
        Point3(0.0f, 0.0f, 4.0f),
        Vec3(0.0f, 180.0f, 0.0f),
        Vec3(2.0f, 2.0f, 2.0f)
    );
    scene.addObject(
        std::make_unique<Mesh>(
            meshTransform,
            data,
            red
        )
    );
    
    // Plane
    Transform planeTransform(
        Point3(0.0f, -2.0f, 8.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
    scene.addObject(
        std::make_unique<Plane>(
            planeTransform,
            Vec3(0.0f, 1.0f, 0.0f),
            white
        )
    );

    // Camera
    Camera mainCam(
        Point3(0.0f, 0.0f, 0.0f),
        Vec3(0.0f, 0.0f, 1.0f),
        90.0f,
        static_cast<float>(width) / height,
        width,
        height
    );

    // light
    PointLight light(
        Point3(0.0f, 5.0f, -2.0f),
        1.275f,
        Color(1.0f, 1.0f, 1.0f)
    );

    return SceneConfig(
        std::move(scene),
        mainCam,
        light
    );
}

SceneConfig createBunnyScene(
    unsigned int width,
    unsigned int height
) {
    Scene scene;

    // Materials
    Material white;
    white.albedo = Color(1.0f, 1.0f, 1.0f);

    Material bunnyMaterial;
    bunnyMaterial.albedo = Color::fromSrgb(0.784f, 0.588f, 0.392f);

    // stanford-bunny
    OBJData data = loadOBJ("objects/stanford-bunny.obj");
    Transform bunnyTransform(
        Point3(0.6f, -2.9f, 5.5f),
        Vec3(0.0f, 180.0f, 0.0f),
        Vec3(-30.0f, 30.0f, 30.0f)
    );
    scene.addObject(
        std::make_unique<Mesh>(
            bunnyTransform,
            data,
            bunnyMaterial
        )
    );

    // Plane
    Transform planeTransform(
        Point3(0.0f, -2.0f, 8.0f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
    scene.addObject(
        std::make_unique<Plane>(
            planeTransform,
            Vec3(0.0f, 1.0f, 0.0f),
            white
        )
    );

    // Camera
    Camera mainCam(
        Point3(0.0f, 1.0f, 0.0f),
        Vec3(0.0f, -0.3f, 1.0f),
        90.0f,
        static_cast<float>(width) / height,
        width,
        height
    );

    // light
    PointLight light(
        Point3(0.0f, 5.0f, -2.0f),
        1.275f,
        Color(1.0f, 1.0f, 1.0f)
    );

    return SceneConfig(
        std::move(scene),
        mainCam,
        light
    );
}

SceneConfig createReflectionsScene(
    unsigned int width,
    unsigned int height
) {
    Scene scene;

    // Materials
    Material mirror;
    mirror.albedo = Color(0.8f, 0.8f, 0.8f);
    mirror.reflectivity = 0.98f;

    Material red;
    red.albedo = Color::fromSrgb(0.784f, 0.196f, 0.196f);

    Material blue;
    blue.albedo = Color::fromSrgb(0.45f, 0.65f, 0.95f);

    Material green;
    green.albedo = Color::fromSrgb(0.196f, 0.784f, 0.314f);

    Material white;
    white.albedo = Color(1.0f, 1.0f, 1.0f);

    // Large reflective sphere
    Transform mirrorSphereTransform(
        Point3(-2.0f, -0.5f, 4.5f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
    scene.addObject(
        std::make_unique<Sphere>(
            mirrorSphereTransform,
            1.0f,
            mirror
        )
    );

    // Reflective AABB
    Transform boxTransform(
        Point3(2.8f, 0.2f, 4.0f),
        Vec3(5.0f, 65.0f, 0.0f),
        Vec3(3.0f, 2.0f, 0.1f)
    );
    scene.addObject(
        std::make_unique<AABB>(
            boxTransform,
            0.8f,
            mirror
        )
    );

    // Colored sphere
    Transform redSphereTransform(
        Point3(-2.2f, -0.9f, 2.3f),
        Vec3(0.0f, 0.0f, 0.0f),
        Vec3(1.0f, 1.0f, 1.0f)
    );
    scene.addObject(
        std::make_unique<Sphere>(
            redSphereTransform,
            0.6f,
            red
        )
    );

    // Suzanne mesh
    OBJData data = loadOBJ("objects/suzanne.obj");
    Transform monkeyTransform(
        Point3(0.8f, -0.8f, 2.0f),
        Vec3(0.0f, 180.0f, 0.0f),
        Vec3(0.7f, 0.7f, 0.7f)
    );
    scene.addObject(
        std::make_unique<Mesh>(
            monkeyTransform,
            data,
            green
        )
    );

    // Plane
    Transform planeTransform(
        Point3(0.0f, -1.8f, 0.0f),
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

    // camera
    Camera mainCam(
        Point3(0.0f, 1.0f, 0.0f),
        Vec3(0.0f, -0.5f, 1.0f),
        90.0f,
        static_cast<float>(width) / height,
        width,
        height
    );

    // light
    PointLight light(
        Point3(0.0f, 5.0f, -2.0f),
        1.275f,
        Color(1.0f, 1.0f, 1.0f)
    );

    return SceneConfig(
        std::move(scene),
        mainCam,
        light
    );
}