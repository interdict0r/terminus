#pragma once

class grenade_info {
public:
	vec3_t position;
	float curtime;
	std::string grenade;

	grenade_info(vec3_t position, float curtime, std::string grenade) {
		this->position = position;
		this->curtime = curtime;
		this->grenade = grenade;
	}
};
extern std::vector<grenade_info> grenadeinfo;

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace scene_end {
		using fn = void(__thiscall*)(void*);
		static void __stdcall hook();
	}

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace do_post_screen_effects {
		using fn = int(__thiscall*)(void*, int);
		static int __stdcall hook(int value);
	};
}
