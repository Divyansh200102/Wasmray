#include <vector>
#include <cmath>
#include <cstdint>
#include <emscripten/emscripten.h>
#include "Vector3.h"

const int WIDTH = 800;
const int HEIGHT = 600;

uint8_t buffer[WIDTH * HEIGHT * 4];

struct Sphere {
    Vector3 center;
    float radius;
    Vector3 color;
};

std::vector<Sphere> spheres = {
    { Vector3(0, 0, -5), 1.0f, Vector3(1.0f, 0.2f, 0.2f) },
    { Vector3(-2, 1, -6), 1.5f, Vector3(0.2f, 1.0f, 0.2f) },
    { Vector3(2, -0.5, -4), 0.8f, Vector3(0.2f, 0.2f, 1.0f) }
};

bool intersect(const Vector3& origin, const Vector3& dir, const Sphere& s, float& t) {
    Vector3 oc = origin - s.center;
    float a = dir.dot(dir);
    float b = 2.0f * oc.dot(dir);
    float c = oc.dot(oc) - s.radius * s.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;
    
    t = (-b - std::sqrt(discriminant)) / (2.0f * a);
    return t > 0;
}

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    uint8_t* getBuffer() {
        return buffer;
    }

    EMSCRIPTEN_KEEPALIVE
    void render(float time) {
        Vector3 lightDir = Vector3(std::sin(time), 1, -1).normalize();

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                float u = (float)x / WIDTH * 2.0f - 1.0f;
                float v = (float)y / HEIGHT * 2.0f - 1.0f;
                u *= (float)WIDTH / HEIGHT; 
                v *= -1; 

                Vector3 rayOrigin(0, 0, 0);
                Vector3 rayDir = Vector3(u, v, -1).normalize();

                Vector3 pixelColor(0.1f, 0.1f, 0.1f);
                float closestT = 10000.0f;

                for (const auto& sphere : spheres) {
                    float t;
                    if (intersect(rayOrigin, rayDir, sphere, t)) {
                        if (t < closestT) {
                            closestT = t;
                            
                            Vector3 hitPoint = rayOrigin + rayDir * t;
                            Vector3 normal = (hitPoint - sphere.center).normalize();
                            
                            float intensity = std::max(0.0f, normal.dot(lightDir));
                            pixelColor = sphere.color * intensity;
                            
                            pixelColor = pixelColor + Vector3(0.1f, 0.1f, 0.1f);
                        }
                    }
                }

                int index = (y * WIDTH + x) * 4;
                buffer[index + 0] = (uint8_t)std::min(255.0f, pixelColor.x * 255);
                buffer[index + 1] = (uint8_t)std::min(255.0f, pixelColor.y * 255);
                buffer[index + 2] = (uint8_t)std::min(255.0f, pixelColor.z * 255);
                buffer[index + 3] = 255;
            }
        }
    }
}
