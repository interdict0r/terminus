#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"

namespace misc {
	void clan_tag();
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void edge_jump(c_usercmd* cmd);
		void fastduck(c_usercmd* cmd);
	};
}

namespace ragebot {
	player_t* GetBestTarget(c_usercmd* cmd);
	void aimbot(c_usercmd* cmd);
}

namespace nade_pred {
	void tick(int buttons);
	void view();
	void paint();

	void setup(player_t* pl, vec3_t& vecSrc, vec3_t& vecThrow, const vec3_t& angEyeAngles);
	void simulate(vec3_t& Angles, player_t* pLocal);
	int step(vec3_t& vecSrc, vec3_t& vecThrow, int tick, float interval);
	bool check_detonate(const vec3_t& vecThrow, const trace_t& tr, int tick, float interval);
	void trace_hull(vec3_t& src, vec3_t& end, trace_t& tr);
	void add_gravity_move(vec3_t& move, vec3_t& vel, float frametime, bool onground);
	void push_entity(vec3_t& src, const vec3_t& move, trace_t& tr);
	void resolve_fly_collision_custom(trace_t& tr, vec3_t& vecVelocity, float interval);
	int physics_clip_velocity(const vec3_t& in, const vec3_t& normal, vec3_t& out, float overbounce);

	enum {
		ACT_NONE,
		ACT_LOB,
		ACT_DROP,
		ACT_THROW
	};
	static inline int act = 0;
	static inline int type = 0;
	static inline std::vector<vec3_t> path;
	static inline std::vector<std::pair<vec3_t, vec3_t>> other_collisions;
	static inline color tracer_color = color(255, 255, 0, 255);
	static inline bool firegrenade_didnt_hit = false;
};

namespace glow {
	void run();
};