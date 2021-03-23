#include "framework.hpp"
#include "variables.hpp"

POINT cursor;
POINT cursor_corrected;

std::string GetKeyName(const int VirtualKey) {
	auto Code = MapVirtualKeyA(VirtualKey, MAPVK_VK_TO_VSC);

	int Result;
	char Buffer[128];

	switch (VirtualKey) {
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_RCONTROL: case VK_RMENU:
	case VK_LWIN: case VK_RWIN: case VK_APPS:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		Code |= KF_EXTENDED;
	default:
		Result = GetKeyNameTextA(Code << 16, Buffer, 128);
	}

	if (Result == 0) {
		switch (VirtualKey) {
		case VK_XBUTTON1:
			return "[MOUSE 4]";
		case VK_XBUTTON2:
			return "[MOUSE 5]";
		case VK_LBUTTON:
			return "[MOUSE 1]";
		case VK_MBUTTON:
			return "[MOUSE 3]";
		case VK_RBUTTON:
			return "[MOUSE 2]";
		default:
			return "[_]";
		}
	}

	auto transformer = std::string("[") + std::string(Buffer) + std::string("]");
	std::transform(transformer.begin(), transformer.end(), transformer.begin(), ::toupper);
	return transformer;
}

void menu_framework::group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool tabs) {
	render::draw_filled_rect(x, y, w, h, color(47, 50, 66, 200));

	int width = render::get_text_size(render::fonts::watermark_font, string).x;
	if (tabs == true)
		render::draw_outline(x, y, w, h, color(28, 30, 39, 255));
	else {
		render::draw_filled_rect(x, y, 8, 1, color(28, 30, 39, 255));
		render::draw_filled_rect(x + width + 23, y, w - width - 23, 1, color(28, 30, 39, 255));
		render::draw_filled_rect(x, y, 1, h, color(28, 30, 39, 255));
		render::draw_filled_rect(x, y + h, w, 1, color(28, 30, 39, 255));
		render::draw_filled_rect(x + w, y, 1, h + 1, color(28, 30, 39, 255));

		render::text(x + 15, y - 6, font, string, false, color::white(255));
	}
}

void menu_framework::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, std::int32_t& tab, std::int32_t count, bool show_outline) {
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
		tab = count;

	// tab background
	if (show_outline)
		render::draw_rect(x, y, w, h, tab == count ? color(28, 30, 39, 255) : color(75, 85, 103, 255));

	// tab label
	render::text(x - render::get_text_size(font, string).x / 2 + 50, y + h / 2 - 8, font, string, false, show_outline ? color::white() : tab == count ? color(222, 60, 60, 255) : color::white(200));
	render::filledcircle(x + 13, y + 15, 50, 5, tab == count ? color(222, 60, 60, 255) : color(66, 70, 95, 255));
}

void menu_framework::hotkey(std::int32_t x, std::int32_t y, unsigned long font, const std::string string, bool& value) {
	GetCursorPos(&cursor);

	int w = 10, h = 10;
	bool dropdown = false;	

	if ((cursor.x > x - 4) && (cursor.x < x + w + 22) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON))
		value = true;

	if ((cursor.x > x - 4) && (cursor.x < x + w + 22) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_RBUTTON))
		dropdown = 1;

	if (GetAsyncKeyState(VK_ESCAPE))
		value = false;

	render::draw_filled_rect(x - 2, y - 5, w + 22, h + 8, color(32, 35, 45, 255));

	if (value) {
		render::draw_rect(x - 2, y - 5, w + 22, h + 8, color(28, 30, 39, 255));
	}

	if (dropdown > 0) {
		render::draw_filled_rect(x + 20, y + 20, w + 20, h + 6, color(32, 35, 45, 255));
	}

	render::text(x, y - 3, font, value ? "   ..." : "    -", false, value ? color::white(255) : color::white(100));
}

void menu_framework::check_box(std::int32_t x, std::int32_t y, unsigned long font, const std::string string, bool& value) {
	GetCursorPos(&cursor);

	int w = 10;

	int width = render::get_text_size(render::fonts::larger, string).x;

	if ((cursor.x > x - 4) && (cursor.x < x + 14 + width) && (cursor.y > y) && (cursor.y < y + 10) && GetAsyncKeyState(VK_LBUTTON) & 1)
		value = !value;

	// background
	render::draw_filled_rect(x - 2, y - 2, w + 2, 12, color(32, 35, 45, 255));

	// checkbox enabled
	if (value) {
		render::draw_filled_rect(x - 1, y - 1, w, 10, color(222, 60, 60, 255));
		render::draw_rect(x - 2, y - 2, w + 2, 12, color(28, 30, 39, 255));
	}

	// checkbox label
	render::text(x + 20, y - 3, font, string, false, color::white());
}

void menu_framework::slider(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, float& value, float min_value, float max_value) {
	GetCursorPos(&cursor);
	
	int ix = x + 140;
	int yi = y + 4;
	
	if ((cursor.x > ix) && (cursor.x < ix + position) && (cursor.y > yi) && (cursor.y < yi + 6) && (GetAsyncKeyState(VK_LBUTTON)))
		value = (cursor.x - ix) / (float(position) / float(max_value));

	// slider background
	render::draw_filled_rect(ix, yi, position, 6, color(34, 35, 40, 255));
	render::draw_filled_rect(ix, yi, value * (float(position) / float(max_value)), 6, color(222, 60, 60, 255));

	// slider label
	render::text(x + 2, y - 1, font, (std::stringstream{ } << string << ": " <<  std::setprecision(3) << value).str(), false, color::white());
}

void menu_framework::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);
	
	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}
	
	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}
	
	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}
}
