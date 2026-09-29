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

void load_model(const std::string& model_path, std::vector<Polygon>& polygons);

using high_res_clock = std::chrono::high_resolution_clock;

int main() {
    constexpr int w = 800;
    constexpr int h = 600;

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
    load_model(model_path, polygons); // TODO: Что делать в случае ошибки/исключения?
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

    float two_pi = static_cast<float>(std::numbers::pi * 2.0);
    float angle_rad_accum = 0.0f;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
         
        angle_rad_accum += 0.01f;
        angle_rad_accum = std::fmod(angle_rad_accum, two_pi);
        if (angle_rad_accum < 0.0f) {
            angle_rad_accum += two_pi;
        }

        clear_framebuffer(Color{ 255, 255, 255, 255 }, framebuffer);
        clear_z_buffer(1.0f, z_buffer);

        Mat4f translation = Mat4f::create_translation(Vec3f{ 0.0f, -1.3f, -6.5f });
        Mat4f rotation_x = Mat4f::create_rotation_x(17.0f * static_cast<float>(std::numbers::pi / 180.0));
        Mat4f rotation_y = Mat4f::create_rotation_y(angle_rad_accum);
        Mat4f rotation_z = Mat4f::create_rotation_z(0.0f);
        //Mat4f scale = Mat4f::create_scale_x(0.03f) * Mat4f::create_scale_y(0.03f) * Mat4f::create_scale_z(0.03f);
        Mat4f scale;
        Mat4f model = translation * rotation_z * rotation_x * rotation_y * scale;
        float fov_vert_rad = static_cast<float>(45.0 * (std::numbers::pi / 180.0));
        float aspect_ratio = static_cast<float>(framebuffer.w) / framebuffer.h;
        MVP mvp{
            model,
            Mat4f::create_identity(),
            Mat4f::create_perspective(fov_vert_rad, aspect_ratio, 0.1f, 10.0f),
            -0.1f,
            -10.0f,
            fov_vert_rad / 2.0f
        };
        //rasterize_polygons_flat_shaded_textured_affine(polygons, house_texture, light, framebuffer, z_buffer, mvp);
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
