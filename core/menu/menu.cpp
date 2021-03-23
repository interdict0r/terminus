#include "menu.hpp"

// todo auto elements positioning

auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, color gradient, const std::string& name) {
		render::draw_filled_rect(x, y, w, h, bg);
		render::draw_filled_rect(x, y, w, 30, header_text);
		render::gradientline(x, y + 27, w, 2, header_line, gradient);
		render::text(x + 10, y + 2, render::fonts::largerer, name, false, color::white());
};

void menu::render() {
	if (!variables::menu::opened)
		return;

	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h, color(65, 69, 89, 200), color(43, 46, 60, 220), color(222, 60, 60, 255), color(52, 134, 235, 255), "terminus");
	
	menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, 100, variables::menu::h - 45, render::fonts::larger, "", true); {
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) - 35, 100, 30, render::fonts::larger, "Aimbot", menu::current_tab, 0, false);
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2), 100, 30, render::fonts::larger, "Visuals", menu::current_tab, 1, false);
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) + 35, 100, 30, render::fonts::larger, "Misc", menu::current_tab, 2, false);
	}

	switch (current_tab) {
	case 0:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 212, variables::menu::h - 45, render::fonts::watermark_font, "Ragebot", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, render::fonts::watermark_font, "Enable Aimbot", variables::aimbot);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 65, render::fonts::watermark_font, "On Key", variables::onkey);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 85, render::fonts::watermark_font, "Visible Only", variables::aimbot_isvisiblecheck);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, render::fonts::watermark_font, "Silent", variables::silent);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 125, render::fonts::watermark_font, "Team-check", variables::teamcheck);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 145, render::fonts::watermark_font, "Auto fire", variables::autoshoot);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 165, render::fonts::watermark_font, "Auto scope", variables::autoscope);
		}
		break;
	case 1:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 212, variables::menu::h - 45, render::fonts::watermark_font, "Player", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, render::fonts::watermark_font, "Enable", variables::enableesp);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 65, render::fonts::watermark_font, "Box", variables::box);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 85, render::fonts::watermark_font, "Name", variables::name);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, render::fonts::watermark_font, "Health", variables::health);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 125, render::fonts::watermark_font, "Weapon", variables::weapon);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 145, render::fonts::watermark_font, "Ammo", variables::ammo);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 165, render::fonts::watermark_font, "Flags", variables::flags);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 185, render::fonts::watermark_font, "Watermark", variables::watermark);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 205, render::fonts::watermark_font, "Radar reveal", variables::radar);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 225, render::fonts::watermark_font, "Grenade prediction", variables::grenadetraj);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 245, render::fonts::watermark_font, "Team check (put under enable)", variables::team);
		}
		menu_framework::group_box(variables::menu::x + 325, variables::menu::y + 35, 212, variables::menu::h / 2 - 50, render::fonts::watermark_font, "World", false); {
			menu_framework::check_box(variables::menu::x + 335, variables::menu::y + 45, render::fonts::watermark_font, "Grenade ESP (todo)", variables::grenade);
			menu_framework::check_box(variables::menu::x + 335, variables::menu::y + 65, render::fonts::watermark_font, "Bomb timer", variables::bomb);
			menu_framework::check_box(variables::menu::x + 335, variables::menu::y + 85, render::fonts::watermark_font, "Dropped weapons", variables::dropped);
		}
		menu_framework::group_box(variables::menu::x + 325, variables::menu::y + 345, 212, 330, render::fonts::watermark_font, "Removals", false); {
			menu_framework::check_box(variables::menu::x + 335, variables::menu::y + 355, render::fonts::watermark_font, "No flash", variables::noflash);
		}
		break;
	case 2:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 212, variables::menu::h - 45, render::fonts::watermark_font, "Movement", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, render::fonts::watermark_font, "Bhop", variables::bhop);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 65, render::fonts::watermark_font, "Fast-duck", variables::fastduck);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 85, render::fonts::watermark_font, "Clantag spammer", variables::clantag);
			menu_framework::hotkey(variables::menu::x + 120, variables::menu::y + 105, render::fonts::watermark_font, "Clantag spammer", variables::clantag);
		}
		break;
	}

	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
}

void menu::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}