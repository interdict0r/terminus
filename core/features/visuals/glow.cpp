#include "../features.hpp"

void glow::run() {
	if (!variables::enableesp)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	i_material* mat_glow = interfaces::material_system->find_material("dev/glow_color", TEXTURE_GROUP_OTHER, true);
	interfaces::model_render->override_material(mat_glow);

	for (size_t i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glow = interfaces::glow_manager->objects[i];
		if (glow.unused())
			continue;

		auto glow_entity = reinterpret_cast<player_t*>(glow.entity);
		auto client_class = glow_entity->client_class();
		if (!glow_entity || glow_entity->dormant())
			continue;


		switch (client_class->class_id) {
		case class_ids::ccsplayer:
				glow.set(255, 0, 0, 255);
			break;
		case class_ids::cplantedc4:
				glow.set(255, 255, 0, 255);
			break;

		case class_ids::chegrenade:
		case class_ids::cflashbang:
		case class_ids::cmolotovgrenade:
		case class_ids::cmolotovprojectile:
		case class_ids::cincendiarygrenade:
		case class_ids::cdecoygrenade:
		case class_ids::cdecoyprojectile:
		case class_ids::csmokegrenade:
		case class_ids::csmokegrenadeprojectile:
		case class_ids::particlesmokegrenade:
		case class_ids::cbasecsgrenade:
		case class_ids::cbasecsgrenadeprojectile:
		case class_ids::cbasegrenade:
		case class_ids::cbaseparticleentity:
		case class_ids::csensorgrenade:
		case class_ids::csensorgrenadeprojectile:
			glow.set(0, 255, 0, 255);
			break;
		}

		if (client_class->class_id == cak47 || client_class->class_id == cdeagle || client_class->class_id == cc4 || client_class->class_id == ceconentity ||
			client_class->class_id >= cweaponaug && client_class->class_id <= cweaponxm1014)
			glow.set(0, 255, 255, 255);
	}
}