#include "local.h"
#include "scene_ids.h"
#include "call.h"
#include "system/frame.h"

struct SlideWork {
  u8 unk000[0x1cc];
  struct Actor *camera;
};

s32 Func_02003d2c(struct Actor *object, s32 *position);
void Func_02002bd4(struct Actor *object);
void Func_080d489c(s32 id, s32 effect);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003cc4(struct Actor *object, s32 animation);
void Script_WaitForEventTimeout(struct Actor *object);

void Func_02002ca4(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  s32 position[3];
  s32 hit;

  {
    struct SlideWork *work = Ram_HeapSlots->eventWork;

    id = gMainRoutineWork.currentOwner;
    center = work->camera;
  }
  actor = Actor_GetById(id);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      actor->terrainHeight == 0)
    return;
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x20000;
  hit = Func_02003d2c(actor, position);
  if ((gFrameCount & 4) == 0)
    Func_02002bd4(actor);
  if (hit < 0) {
    /* FAKEMATCH(wrapper): Call2 kept; as a direct call, 2 of 151 instructions
     * differ in tla-en. */
    Call2(Func_080d489c, id, 258);
    Actor_SetMoveTarget(actor, actor->x, actor->y, actor->z + 0x80000);
    Func_02003cc4(actor, 49);
    Script_WaitForEventTimeout(actor);
    do {
      wait(1);
    } while (actor->y != actor->terrainHeight);
    Func_02002bd4(actor);
    Func_02003cc4(actor, 49);
    wait(3);
    return;
  }
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x80000;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x + 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x - 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  center->z += 0x20000;
  actor->z += 0x20000;
  actor->angle = 0x4000;
}
