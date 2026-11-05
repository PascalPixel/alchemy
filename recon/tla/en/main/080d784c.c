// Draft, differ 53 of 107, 21 normalized, 5 with registers ignored (tla-en, 2026-11-05): flags and the raw halfword read apart, the scene as 0xfff and its raw value. Left: the native holds the scene's mask in r10 before the and (a one-pass block around it gives 51, shape 3), and the temporaries after it take r1, r2 and r0 where this has r0, r1 and r2.
// Draft, differ 57 of 107, skeleton gap 9 (tla-en, 2026-11-01)
// Twin of The Broken Seal's Actor_PlaceMapActors (the same pending-event test,
// flag 0x109, range 300 to 380, placement match and 32-pixel lift); ⚓️ places
// ids 8 to 79 and calls Actor_SetMode. Both natives share the head's
// "mov sl, r3; mov r0, sl; ands r0, r2; mov sl, r0".
// Left: here reload inherits the 0xfff mask's r3, so the head is two
// instructions short and every later reload register runs one behind. That is
// the 57: the owner's offset lands in r0, not the 0x256 constant's register, so
// move2add's "subs r2, #66" does not form (it would with the rotation back).
// Owner's address set after id = 8, as loop.c's hoist leaves it; set before the
// loop's counter the gap is 11, set at the top of the function the gap is 10
// and differ 32 (the rotation happens to line up), read plainly in the loop
// it is not hoisted, differ 68.
// Tried without gain: a pending pointer, a held mask, a shared s32 or s16 read,
// scene first, scene assigned apart from its read, (u16) casts, signed scene,
// 0xfff on the left, unsigned flags, the twin's plain global (the pool then
// holds the work's 0x254 and differs 64), locals at the block's top.
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
  s32 flags = party->pendingDjinnEvent;
  u32 raw = (u16)party->pendingDjinnEvent;
  u32 scene;
  s32 id;

  flags &= 0xf000;
  scene = 0xfff;
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
