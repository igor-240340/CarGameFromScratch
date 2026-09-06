#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include "Vec3f.h"
#include "Mat4f.h"

struct TexCoord {
	float u;
	float v;
};

struct Vertex {
	Vec3f pos;
	TexCoord tex_coord;
};

struct Color {
	std::uint8_t r;
	std::uint8_t g;
	std::uint8_t b;
	std::uint8_t a;
};

struct Polygon {
	std::array<Vertex, 3> vertices;
	Color albedo_color;
	Vec3f normal;
};

struct Light {
	Vec3f dir;
};

struct Framebuffer {
	int w;
	int h;
	std::vector<std::uint8_t> rgba_array;
};

struct ZBuffer {
	int w;
	int h;
	std::vector<float> depth_array;
};

struct Texture {
	unsigned int w;
	unsigned int h;
	const std::uint8_t* rgba_array;
};

struct MVP {
	Mat4f model;
	Mat4f view;
	Mat4f proj;
	float near_plane;
	float far_plane;
	float fov_vert_half_rad;
};


void draw_line_dda_z(float x0, float y0, float z0, float x1, float y1, float z1, Color color, Framebuffer& framebuffer, ZBuffer& z_buffer);
bool clip_line_coh_suth_z(float& x0, float& y0, float& z0, float& x1, float& y1, float& z1, const Framebuffer& framebuffer);

void rasterize_polygons_flat_shaded_textured_affine(const std::vector<Polygon>& polygons, Texture texture, const Light& light, Framebuffer& framebuffer, ZBuffer& z_buffer, const MVP& mvp);
void draw_polygon_flat_shaded_textured_affine(Polygon polygon_screen, Texture texture, float light_intensity, Framebuffer& framebuffer, ZBuffer& z_buffer);
void draw_flat_bottom_polygon_flat_shaded_textured_affine(Polygon polygon_screen, Texture texture, float light_intensity, Framebuffer& framebuffer, ZBuffer& z_buffer);
void draw_flat_top_polygon_flat_shaded_textured_affine(Polygon polygon_screen, Texture texture, float light_intensity, Framebuffer& framebuffer, ZBuffer& z_buffer);
void draw_flat_bottom_polygon_flat_shaded_textured_affine_no_crack(Polygon polygon_screen, float inv_slope, Texture texture, float light_intensity, Framebuffer& framebuffer, ZBuffer& z_buffer);
void draw_flat_top_polygon_flat_shaded_textured_affine_no_crack(Polygon polygon_screen, float inv_slope, const Vertex& v_top, Texture texture, float light_intensity, Framebuffer& framebuffer, ZBuffer& z_buffer);
Color get_texture_pixel_color(Texture texture, int pixel_x, int pixel_y);

void rasterize_polygons_flat_shaded(const std::vector<Polygon>& polygons, const Light& light, Framebuffer& framebuffer, ZBuffer& z_buffer, const MVP& mvp);
void draw_polygon_solid(Polygon polygon_screen, Framebuffer& framebuffer, ZBuffer& z_buffer);
void draw_flat_bottom_polygon_solid(Polygon polygon_screen, Framebuffer& framebuffer, ZBuffer& z_buffer);
void draw_flat_top_polygon_solid(Polygon polygon_screen, Framebuffer& framebuffer, ZBuffer& z_buffer);

void draw_flat_bottom_polygon_solid_no_crack(Polygon polygon_screen, float inv_slope, Framebuffer& framebuffer, ZBuffer& z_buffer);
void draw_flat_top_polygon_solid_no_crack(Polygon polygon_screen, float inv_slope, const Vertex& v_top, Framebuffer& framebuffer, ZBuffer& z_buffer);

bool perform_depth_test(int frag_x, int frag_y, float frag_z, ZBuffer& z_buffer);
void set_pixel_color(int x, int y, Color color, Framebuffer& framebuffer);

void clear_framebuffer(Color color, Framebuffer& framebuffer);
void clear_z_buffer(float depth_value, ZBuffer& z_buffer);
