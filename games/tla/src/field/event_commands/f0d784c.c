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
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);

void Func_080d784c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u32 raw = (u16)party->pendingDjinnEvent;
  s32 flags = party->pendingDjinnEvent & 0xf000;
  u32 scene = 0xfff;
  s32 id;

  /* FAKEMATCH(barrier): the form kept; as scene &= raw without the asm, 57 of
   * 107 instructions differ in tla-en. */
  asm("" : "+r"(scene));
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
