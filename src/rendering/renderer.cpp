#include <rendering/renderer.hpp>

Renderer::Renderer(const Camera& cam_, Image& img_) : cam(cam_), img(img_) {

}

void Renderer::render() {
    for(size_t y = 0; y < img.getHeight(); ++y){
        for(size_t x = 0; x < img.getWidth(); ++x){
            Ray ray = cam.generateRay(x, y);

            Color color;
            // Convert ray direction from [-1, 1] to [0, 255].
            color.r = (ray.direction.x + 1.0f) * 127.5f;
            color.g = (ray.direction.y + 1.0f) * 127.5f;
            color.b = (ray.direction.z + 1.0f) * 127.5f;

            img.setPixel(x, y, color);
        }
    }
}