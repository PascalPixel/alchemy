#ifndef ALCHEMY_COLOSSO_GAUGE_H
#define ALCHEMY_COLOSSO_GAUGE_H

#include "types.h"
#include "layout_guard.h"

struct ColossoGaugeSprite {
  s32 link;
  s32 attr;
  s32 tile;
};

struct ColossoGaugeWork {
  struct ColossoGaugeSprite sprites[18];
  s16 vramBlock;
  s16 level;
  s16 hold;
  s16 playerActor;
  s16 rivalActor;
  u16 finishItemActorA;
  u16 finishItemActorB;
  s16 segments;
  s32 finishX;
  s32 finishZ;
};

LAYOUT_SIZE_GUARD(ColossoGaugeSprite_Size, struct ColossoGaugeSprite, 12);
LAYOUT_SIZE_GUARD(ColossoGaugeWork_Size, struct ColossoGaugeWork, 240);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_vram_block, struct ColossoGaugeWork,
                    vramBlock, 216);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_level, struct ColossoGaugeWork, level,
                    218);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_hold, struct ColossoGaugeWork, hold, 220);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_player_actor, struct ColossoGaugeWork,
                    playerActor, 222);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_rival_actor, struct ColossoGaugeWork,
                    rivalActor, 224);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_finish_item_actor_a,
                    struct ColossoGaugeWork, finishItemActorA, 226);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_finish_item_actor_b,
                    struct ColossoGaugeWork, finishItemActorB, 228);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_segments, struct ColossoGaugeWork,
                    segments, 230);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_finish_x, struct ColossoGaugeWork, finishX,
                    232);
LAYOUT_OFFSET_GUARD(ColossoGaugeWork_finish_z, struct ColossoGaugeWork, finishZ,
                    236);

#endif
