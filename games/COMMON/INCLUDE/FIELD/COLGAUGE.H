#ifndef ALCHEMY_COLGAUGE_H
#define ALCHEMY_COLGAUGE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* The race gauge and its sprites: the work begins with their eighteen
 * records, and the stage scenes keep their own state after them. */
struct KorosseoGaugeSprite {
    s32 link;
    s32 attr;
    s32 tile;
};

struct KorosseoGaugeWork {
    struct KorosseoGaugeSprite sprites[18];
    s16 vram_block;
    /* How far the gauge has slid in from the top, 0 to 2. */
    s16 level;
    s16 hold;
    s16 player_actor;
    s16 rival_actor;
    u16 finish_item_actor_a;
    u16 finish_item_actor_b;
    /* Middle pieces on each side of the gauge. */
    s16 segments;
    s32 finish_x;
    s32 finish_z;
};

LAYOUT_SIZE_GUARD(KorosseoGaugeSprite_Size, struct KorosseoGaugeSprite, 12);
LAYOUT_SIZE_GUARD(KorosseoGaugeWork_Size, struct KorosseoGaugeWork, 240);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_vram_block, struct KorosseoGaugeWork, vram_block, 216);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_level, struct KorosseoGaugeWork, level, 218);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_hold, struct KorosseoGaugeWork, hold, 220);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_player_actor, struct KorosseoGaugeWork, player_actor, 222);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_rival_actor, struct KorosseoGaugeWork, rival_actor, 224);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_finish_item_actor_a, struct KorosseoGaugeWork, finish_item_actor_a, 226);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_finish_item_actor_b, struct KorosseoGaugeWork, finish_item_actor_b, 228);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_segments, struct KorosseoGaugeWork, segments, 230);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_finish_x, struct KorosseoGaugeWork, finish_x, 232);
LAYOUT_OFFSET_GUARD(KorosseoGaugeWork_finish_z, struct KorosseoGaugeWork, finish_z, 236);

#endif
