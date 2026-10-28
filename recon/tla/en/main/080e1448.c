// Differ 13 of 89 (2026-10-26); the arc sprites are now the field effects'
// EffectSprite (effect_sprite.h), the three bitfields its OAM's palette256,
// palette and priority (no change in the count).
// Near miss, differ 15 of 89 with the age's zero through a temporary (36
// without, when it loads a halfword 0 from the pool): the native adds the
// group's offset to the work (adds r5, r3, r6) where this adds the work to
// it, and copies the -33 mask before its AND; the rest is the pool offsets
// those shift. The group read once into a local measured 36, read at each
// use 55; &sprites[group][0] and a byte offset measured 15.
// BattleEffect_SpawnScaledArcObjects, ⚓️'s arc effect (heap slot 0xf8,
// arc_effect.h). Sets up the current group's two 40-byte arc sprites from the
// effect's tiles (16 by 16, attributes 0x80004000), shown, layer 0 and at the
// target object's priority, then restarts that group's age, records the
// target and flips to the other group. Not ☀️'s function of that name, which
// creates two objects.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "vram_tab.h"
#include "effect_sprite.h"

struct ArcWork {
  struct EffectSprite sprites[2][2];
  s16 age[2];
  s16 resource;
  s16 group;
  struct Actor *target;
};

s32 BattleEffect_GetObjectPriority(struct Actor *object);

void BattleEffect_SpawnScaledArcObjects(struct Actor *target) {
  struct ArcWork *work = Ram_HeapSlots->block62;
  s32 tiles = gResourceTableEntries[work->resource].offset >> 5;
  s32 group = work->group;
  struct EffectSprite *sprite = work->sprites[group];
  s32 i;
  s32 zero;

  for (i = 0; i < 2; i++, sprite++) {
    Func_080eaf98(sprite, 16, 16, 0x80004000, tiles);
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 0;
    sprite->oam.priority = BattleEffect_GetObjectPriority(target);
  }
  /* FAKEMATCH: the age's zero through a temporary, so it is a register
     zero rather than a pool load (differ 36 without) */
  zero = 0;
  work->age[group] = zero;
  work->target = target;
  work->group ^= 1;
}
