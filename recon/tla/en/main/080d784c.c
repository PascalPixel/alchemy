// Differ 6 of 107 (tla-en, 2026-11-08, -mtune=gba).
// 2026-11-07: an empty once-only block after the 0xf000 and (permuter) puts
// the ldrsh before the ldrh as in the native; left is the 0xf000 mask and its
// and, built after the 0xfff where the native builds them first.
// Earlier, differ 8 of 107 (tla-en, 2026-11-06): the same 107 instructions in a different order at the head. Here the halfword's ldrh comes before the ldrsh, and the 0xfff mask is built before the 0xf000 one; the native loads with ldrsh first and builds and applies 0xf000 before 0xfff. Raw read first gives the ldrsh's offset register r1, as in the native. The mask's set apart from its and, with the 0xf000 and between them, keeps reload from reusing r3 for the scene's and, so the and reloads through r0 as in the native and every later reload register lines up (the 0x214 owner offset now comes from "subs r2, #66"). Flags read first, or 0xfff set right before its and, brings back the r3 reuse (differ 56 to 57).
// Earlier: differ 53 of 107 (2026-11-05) with flags and the raw halfword read apart and 0xfff set just before its and; differ 57 (2026-11-01).
// Twin of The Broken Seal's Actor_PlaceMapActors (the same pending-event test,
// flag 0x109, range 300 to 380, placement match and 32-pixel lift); ⚓️ places
// ids 8 to 79 and calls Actor_SetMode. Both natives share the head's
// "mov sl, r3; mov r0, sl; ands r0, r2; mov sl, r0".
// Tried at differ 8's head without gain: the twin's one-expression masks (57),
// scene = raw & scene (8), u16 or s32 raw or scene (70, 10), the and of the
// scene before the flags' (63), 0xfff declared before the reads (22, 73), a
// pending pointer (8).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct ActorDescriptor {
  s16 action;
  s16 scene;
  u8 unk04[4];
  s32 x;
  s32 y;
  s32 z;
};

s32 GameFlag_Test(s32 flag);
struct ActorDescriptor *Scene_FindActorPlacement(s32 id);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x,
                                         s32 y, s32 z);

void Func_080d784c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u32 raw = (u16)party->pendingDjinnEvent;
  s32 flags = party->pendingDjinnEvent;
  u32 scene = 0xfff;
  s32 id;

  flags &= 0xf000;
  // A once-only block: it ends a block, so the flags' ldrsh comes first.
  do {
  } while (0);
  scene &= raw;
  if (GameFlag_Test(0x109) != 0 && flags == 0) {
    flags = scene & 0x800;
    scene &= 0x7ff;
    if (scene - 300 <= 80 && party->sequenceResult > 0) {
      s32 *owner;

      for (id = 8, owner = &party->currentOwner; id <= 79; id++) {
        struct ActorDescriptor *desc = Scene_FindActorPlacement(id);
        struct Actor *obj;

        if (desc == NULL || desc->scene - 48 != scene - 300)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->motionFlags = 3;
          Actor_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct Actor *selection = ObjectTable_Get(*owner);

          Actor_SetPositionAndResetMotionFar(obj, selection->x, selection->y,
                                              selection->z - (32 << 16));
        }
        Actor_SetMode(obj, 1);
      }
    }
  }
}
