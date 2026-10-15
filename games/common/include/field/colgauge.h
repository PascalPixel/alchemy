#ifndef ALCHEMY_COLGAUGE_H
#define ALCHEMY_COLGAUGE_H

#include "types.h"
#include "layout_guard.h"

struct ColossoGaugeSprite
{
	s32 link;
	s32 attr;
	s32 tile;
};

struct ColossoGaugeWork
{
	struct ColossoGaugeSprite sprites[18];
	s16						   vram_block;
	s16						   level;
	s16						   hold;
	s16						   player_actor;
	s16						   rival_actor;
	u16						   finish_item_actor_a;
	u16						   finish_item_actor_b;
	s16						   segments;
	s32						   finish_x;
	s32						   finish_z;
};

LAYOUT_SIZE_GUARD(ColossoGaugeSprite_Size, struct ColossoGaugeSprite, 12);
LAYOUT_SIZE_GUARD(ColossoGaugeWork_Size, struct ColossoGaugeWork, 240);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_vram_block, struct ColossoGaugeWork, vram_block, 216);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_level, struct ColossoGaugeWork, level, 218);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_hold, struct ColossoGaugeWork, hold, 220);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_player_actor, struct ColossoGaugeWork, player_actor, 222);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_rival_actor, struct ColossoGaugeWork, rival_actor, 224);
LAYOUT_OFFSET_GUARD(
	ColossoGaugeWork_finish_item_actor_a, struct ColossoGaugeWork, finish_item_actor_a, 226);
LAYOUT_OFFSET_GUARD(
	ColossoGaugeWork_finish_item_actor_b, struct ColossoGaugeWork, finish_item_actor_b, 228);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_segments, struct ColossoGaugeWork, segments, 230);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_finish_x, struct ColossoGaugeWork, finish_x, 232);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_finish_z, struct ColossoGaugeWork, finish_z, 236);

#endif
