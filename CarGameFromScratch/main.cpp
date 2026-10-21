#include <print>
#include <numbers>
#include <format>
#include <chrono>
#include <cmath>

#include <SFML/Graphics.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Vec3f.h"
#include "Vec4f.h"
#include "Mat4f.h"
#include "graphics.h"
#include "physics.h"

void load_model(const std::string& model_path, std::vector<Polygon>& polygons);

using high_res_clock = std::chrono::high_resolution_clock;

int main() {
    constexpr int w = 1024;
    constexpr int h = 768;
    constexpr sf::Time fixed_dt = sf::seconds(1.0f / 60.0f);

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(w, h)), "Car Game From Scratch");
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);

    sf::Texture framebuffer_texture(sf::Vector2u(w, h));
    sf::Sprite framebuffer_sprite(framebuffer_texture);

	Light light{ Vec3f{0.0f, 0.0f, -1.0f} };
    Framebuffer framebuffer{ w, h, std::vector<std::uint8_t>(w * h * 4) };
    ZBuffer z_buffer{ w, h, std::vector<float>(w * h) };

    std::vector<Polygon> polygons;
    std::string model_path = "assets/brick/brick.obj";
    load_model(model_path, polygons);
    if (polygons.empty()) {
        std::println("Failed to load model: {}", model_path);
        return 1;
    }

    /*
    sf::Image house_texture_image;
    if (!house_texture_image.loadFromFile("assets/house/house.jpg")) {
        std::println("Failed to load texture: assets/house/house.jpg");
        return 1;
    }
    Texture house_texture{
        house_texture_image.getSize().x,
        house_texture_image.getSize().y,
        house_texture_image.getPixelsPtr()
    };
    */

    auto measure_start = high_res_clock::now();
    int frame_count = 0;

    sf::Clock clock;
    sf::Time dt_acc = sf::Time::Zero;

    RigidBody body;
    body.set_mass(100.0f);
    body.set_inertia_tensor(Mat3(Vec3f(3.0f, 0.0f, 0.0f), Vec3f(0.0f, 6.0f, 0.0f), Vec3f(0.0f, 0.0f, 5.0f)));
    body.set_position(Vec3f(0.0f, 0.0f, -15.5f));
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        body.add_force_at_point(Vec3f(0.0f, 5.0f, 0.0f), Vec3f(2.0f, 0.0f, -15.5f));

        sf::Time dt = clock.restart();
        dt_acc += dt;
        while (dt_acc > fixed_dt) {
            body.integrate(fixed_dt.asSeconds());
            dt_acc -= fixed_dt;
        }

        clear_framebuffer(Color{ 255, 255, 255, 255 }, framebuffer);
        clear_z_buffer(1.0f, z_buffer);

        float fov_vert_rad = static_cast<float>(45.0 * (std::numbers::pi / 180.0));
        float aspect_ratio = static_cast<float>(framebuffer.w) / framebuffer.h;
        MVP mvp{
            body.get_tranform(),
            Mat4f::create_identity(),
            Mat4f::create_perspective(fov_vert_rad, aspect_ratio, 0.1f, 50.0f),
            -0.1f,
            -50.0f,
            fov_vert_rad / 2.0f
        };
        rasterize_polygons_flat_shaded(polygons, light, framebuffer, z_buffer, mvp);

        framebuffer_texture.update(framebuffer.rgba_array.data());

        window.clear();
        window.draw(framebuffer_sprite);
        window.display();

        std::chrono::duration<float> elapsed_seconds = high_res_clock::now() - measure_start;
        frame_count++;
        if (elapsed_seconds.count() >= 1.0f) {
            window.setTitle(std::format("Car Game From Scratch | FPS: {}", static_cast<int>(frame_count / elapsed_seconds.count())));
            frame_count = 0;
            measure_start = high_res_clock::now();
        }
    }

    return 0;
}

void load_model(const std::string& model_path, std::vector<Polygon>& polygons) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model_path.c_str())) {
        std::println("tinyobjloader: {}", err);
        return;
    }
    if (!err.empty()) {
        std::println("tinyobjloader: {}", err);
    }

    for (const tinyobj::shape_t& shape : shapes) {
        for (std::size_t i = 0; i + 2 < shape.mesh.indices.size(); i += 3) {
            const tinyobj::index_t& index0 = shape.mesh.indices[i + 0];
            const tinyobj::index_t& index1 = shape.mesh.indices[i + 1];
            const tinyobj::index_t& index2 = shape.mesh.indices[i + 2];

            Vertex v0{ Vec3f{
                attrib.vertices[3 * index0.vertex_index + 0],
                attrib.vertices[3 * index0.vertex_index + 1],
                attrib.vertices[3 * index0.vertex_index + 2]
            }, TexCoord{
                //attrib.texcoords[2 * index0.texcoord_index + 0],
                //attrib.texcoords[2 * index0.texcoord_index + 1]
            } };

            Vertex v1{ Vec3f{
                attrib.vertices[3 * index1.vertex_index + 0],
                attrib.vertices[3 * index1.vertex_index + 1],
                attrib.vertices[3 * index1.vertex_index + 2]
            }, TexCoord{
                //attrib.texcoords[2 * index1.texcoord_index + 0],
                //attrib.texcoords[2 * index1.texcoord_index + 1]
            } };

            Vertex v2{ Vec3f{
                attrib.vertices[3 * index2.vertex_index + 0],
                attrib.vertices[3 * index2.vertex_index + 1],
                attrib.vertices[3 * index2.vertex_index + 2]
            }, TexCoord{
                //attrib.texcoords[2 * index2.texcoord_index + 0],
                //attrib.texcoords[2 * index2.texcoord_index + 1]
            } };

			//polygons.push_back(Polygon{ { v0, v1, v2 }, Color{ 255, 135, 0, 255 } });
			polygons.push_back(Polygon{ { v0, v1, v2 }, Color{ 128, 0, 0, 255 } });
        }
    }
}
