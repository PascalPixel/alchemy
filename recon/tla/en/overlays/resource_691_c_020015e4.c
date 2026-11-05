// Measured under -Os, 5 November 2026: differ 78 (tla-en); notes below may predate it.
// Draft, differ 78 of 153, skeleton gap 7 removed, 3 added (tla-en, 2026-11-01)
//
// An update hook: the object keeps a height set from the object it watches (at
// 0x68 of the effect view) by that object's action value. From 35 to 42 the
// offset grows 8 pixels a step, from 9 to 331 it holds by the byte at 0x62, and
// from 333 for twice that byte it shrinks back; past that the object takes its
// terrain height and clears its hook. Every eighth frame it throws effect 403,
// and it ends by calling Func_020005cc with the watched object's layer and cell.
//
// Left: one cause. The native computes object + 98 once before the 9 to 331
// test (r0) for that arm and the next, lets it die, and computes it again for
// the last test. Written as a local here, gcse's PRE deletes the last
// computation, the local lives on, the test below 333 + 2 * byte needs a fifth
// low register, r5 joins the spill set and reload's rotation shifts every later
// scratch register. Those shifted scratches are what move the pool load in the
// first arm (native r3, an anti-dependence keeps it after the add) and most of
// the differ count. With no local the address lands in each arm instead (gap 12
// and 11, differ 121). The same recomputation is in the sibling
// resource_691_c_02001528.c. gcse.c shows no plain reason for the expression
// not to be available there (it needs a base it cannot copy-propagate or an
// edge from a path without it), and I found no source form.
// The falling arm is (332 - action) * 0x80000 for 78; the native's subtract then
// add of 0xa600000 gives its skeleton as y - (action << 19) + 0xa600000 but only
// after the rotation is right (differ 100, gap 9 and 7 today), so try that first
// once the address is settled.
//
// Measured forms:
// - object must be union FieldObject *, the hook's real type: the union makes
//   its stores alias set 0, so the first y store kills the action reread and
//   survives. As a plain struct pointer both go (differ 135, 2 stores lost).
// - watched->action read as a field at each use. Through an s16 *action local
//   the last read merges into the sign-extended copy (differ 114 to 128).
// - the first arm can be written three ways with the same result: (action - 34)
//   * 0x80000, steps, or one expression. The multiply is kept.
// - a u16 frame local for the tests: 161. Second pointer to the effect view, a
//   two-step or s32 local: all unchanged.
#include "actor.h"
#include "system/field_effect.h"
#include "system/main_routine_work.h"
#include "types.h"
#include "call.h"

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[4];
  void (*unk24)(void);
};

extern u32 gFrameCount;
extern u32 gData_02003d50[];
void Func_020011b8(void);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);

void Func_020015e4(union FieldObject *object) {
  struct Actor *watched = (struct Actor *)object->effect.countdown;
  struct MagmaRockEffectConfig config;
  s32 step;
  s32 mask;

  if (watched->action >= 35 && watched->action <= 42) {
    object->effect.y = watched->y + 0x180000;
    object->effect.y += (watched->action - 34) * 0x80000;
  } else {
    u8 *unk62 = &object->effect.unk5a[8];

    if (watched->action >= 9 && watched->action <= 331) {
      object->effect.y = watched->y + (*unk62 << 20) + 0x180000;
    } else if (watched->action > 332 &&
               watched->action < (*unk62 << 1) + 333) {
      object->effect.y = watched->y + (*unk62 << 20) + 0x180000;
      object->effect.y += (332 - watched->action) * 0x80000;
    } else if ((object->effect.unk5a[8] << 1) + 333 < watched->action) {
      object->effect.y = object->actor.terrainHeight;
      object->effect.update = 0;
    }
  }

  mask = 7;
  step = gFrameCount & mask;
  if (step == 0) {
    config.id = 403;
    config.variant = 10;
    config.kind = 2;
    config.unk24 = Func_020011b8;
    config.data = gData_02003d50;
    Func_0200015c(object->effect.x + 0x10000, object->effect.y - 0x40000,
                  object->effect.z, ((Random16Far() & mask) - 3) << 14,
                  -0x20000, step, 0x1330000, &config);
  }
  Func_020005cc(watched->terrainId, watched->x >> 20, watched->z >> 20,
                (object->effect.y >> 19) + 1);
}
