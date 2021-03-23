#include "renderer.hpp"

unsigned long render::fonts::watermark_font;
unsigned long render::fonts::larger;
unsigned long render::fonts::largerer;
unsigned long render::fonts::pixel;
unsigned long render::fonts::weapon;

void render::initialize() {
	render::fonts::watermark_font = interfaces::surface->font_create();
	render::fonts::larger = interfaces::surface->font_create();
	render::fonts::largerer = interfaces::surface->font_create();
	render::fonts::pixel = interfaces::surface->font_create();
	render::fonts::weapon = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph(render::fonts::watermark_font, "Verdana", 12, 500, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::larger, "Verdana", 16, 400, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::largerer, "Verdana", 24, 400, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::pixel, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_dropshadow);
	interfaces::surface->set_font_glyph(render::fonts::weapon, "Counter-Strike", 28, 600, 0, 0, font_flags::fontflag_dropshadow);

	console::log("[setup] render initialized!\n");
}

void render::draw_line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t *text, bool centered, color color) {
	interfaces::surface->draw_text_font(font);
	int text_width, text_height;

	if (centered) {
		interfaces::surface->get_text_size(font, text, text_width, text_height);
		interfaces::surface->draw_text_pos(x - text_width / 2, y);
	}
	else
		interfaces::surface->draw_text_pos(x, y );

	interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_render_text(text, wcslen(text));
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, std::string text, bool centered, color color) {
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, temp, 128) > 0) {
		interfaces::surface->draw_text_font(font);
		if (centered) {
			interfaces::surface->get_text_size(font, temp, text_width, text_height);
			interfaces::surface->draw_text_pos(x - text_width / 2, y);
		}
		else
			interfaces::surface->draw_text_pos(x, y);
			interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
			interfaces::surface->draw_render_text(temp, wcslen(temp));
	}
}

void render::gradientline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color Top, color Bottom) {
	interfaces::surface->set_drawing_color(Top.r, Top.g, Top.b, Top.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, 255, 255, true);

	interfaces::surface->set_drawing_color(Bottom.r, Bottom.g, Bottom.b, Bottom.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, 0, 255, true);
}

void render::draw_circle_3d(vec3_t position, float radius) {
	vec3_t prev_scr_pos, scr_pos;

	float step = M_PI * 2.0 / 150;

	vec3_t origin = position;

	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step) {
		vec3_t pos(radius * cos(rotation) + origin.x, radius * sin(rotation) + origin.y, origin.z);
		vec3_t tracepos(origin.x, origin.y, origin.z + 2);

		ray_t ray;
		trace_t trace;
		trace_filter filter;

		//bfilter.skip = csgo::local_player;
		ray.initialize(origin, pos);

		interfaces::trace_ray->trace_ray(ray, MASK_SOLID_BRUSHONLY, &filter, &trace);

		if (math::worldtoscreen(trace.end, scr_pos)) {
			if (prev_scr_pos.isvalid()) {
				interfaces::surface->set_drawing_color(255, 255, 255, 255);
				interfaces::surface->draw_line(prev_scr_pos.x, prev_scr_pos.y, scr_pos.x, scr_pos.y);
			}
			prev_scr_pos = scr_pos;
		}
	}
}

void render::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_filled_rectangle(x, y, width, height);
}

void render::filledcircle(std::int32_t x, std::int32_t y, float points, float radius, color col) {
	std::vector<vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(vertex_t(vec2_t(radius * cosf(a) + x, radius * sinf(a) + y)));

	interfaces::surface->set_drawing_color(col.r, col.g, col.b, col.a);
	interfaces::surface->draw_textured_polygon((int)points, vertices.data(), true);
}

void render::draw_outline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_3d_cube(float scalar, vec3_t angles, vec3_t middle_origin, std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha)
{
	vec3_t forward, right, up;
	math::anglevectors(angles, forward, right, up);

	vec3_t points[8];
	points[0] = middle_origin - (right * scalar) + (up * scalar) - (forward * scalar); // BLT
	points[1] = middle_origin + (right * scalar) + (up * scalar) - (forward * scalar); // BRT
	points[2] = middle_origin - (right * scalar) - (up * scalar) - (forward * scalar); // BLB
	points[3] = middle_origin + (right * scalar) - (up * scalar) - (forward * scalar); // BRB

	points[4] = middle_origin - (right * scalar) + (up * scalar) + (forward * scalar); // FLT
	points[5] = middle_origin + (right * scalar) + (up * scalar) + (forward * scalar); // FRT
	points[6] = middle_origin - (right * scalar) - (up * scalar) + (forward * scalar); // FLB
	points[7] = middle_origin + (right * scalar) - (up * scalar) + (forward * scalar); // FRB

	vec3_t points_screen[8];
	for (int i = 0; i < 8; i++)
		if (!math::worldtoscreen(points[i], points_screen[i]))
			return;

	interfaces::surface->set_drawing_color(red, green, blue, alpha);

	// Back frame
	interfaces::surface->draw_line(points_screen[0].x, points_screen[0].y, points_screen[1].x, points_screen[1].y);
	interfaces::surface->draw_line(points_screen[0].x, points_screen[0].y, points_screen[2].x, points_screen[2].y);
	interfaces::surface->draw_line(points_screen[3].x, points_screen[3].y, points_screen[1].x, points_screen[1].y);
	interfaces::surface->draw_line(points_screen[3].x, points_screen[3].y, points_screen[2].x, points_screen[2].y);

	// Frame connector
	interfaces::surface->draw_line(points_screen[0].x, points_screen[0].y, points_screen[4].x, points_screen[4].y);
	interfaces::surface->draw_line(points_screen[1].x, points_screen[1].y, points_screen[5].x, points_screen[5].y);
	interfaces::surface->draw_line(points_screen[2].x, points_screen[2].y, points_screen[6].x, points_screen[6].y);
	interfaces::surface->draw_line(points_screen[3].x, points_screen[3].y, points_screen[7].x, points_screen[7].y);

	// Front frame
	interfaces::surface->draw_line(points_screen[4].x, points_screen[4].y, points_screen[5].x, points_screen[5].y);
	interfaces::surface->draw_line(points_screen[4].x, points_screen[4].y, points_screen[6].x, points_screen[6].y);
	interfaces::surface->draw_line(points_screen[7].x, points_screen[7].y, points_screen[5].x, points_screen[5].y);
	interfaces::surface->draw_line(points_screen[7].x, points_screen[7].y, points_screen[6].x, points_screen[6].y);
}

void render::draw_textured_polygon(std::int32_t vertices_count, vertex_t* vertices, color color) {
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	unsigned int texture_id{};
	if (!texture_id) {
		texture_id = interfaces::surface->create_new_texture_id(true);
		interfaces::surface->set_texture_rgba(texture_id, buf, 1, 1);
	}
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_polygon(vertices_count, vertices);
}

void render::draw_circle(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color) {
	float step = M_PI * 2.0 / segments;
	for (float a = 0; a < (M_PI * 2.0); a += step) {
		float x1 = radius * cos(a) + x;
		float y1 = radius * sin(a) + y;
		float x2 = radius * cos(a + step) + x;
		float y2 = radius * sin(a + step) + y;
		interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
}

vec2_t render::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}