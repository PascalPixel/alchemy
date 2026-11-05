#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "system/vector3.h"

struct Actor *Actor_GetById();
void Func_02003d7c();
s32 Func_02003eac();
void Func_02000ca8();
s32 Func_02003d2c(struct Actor *object, struct Vector3 *target);
void wait();

void Func_02000cc8(s32 trigger) {
  s32 clear = 1;
  struct Actor *object;
  struct Vector3 target;
  s32 result;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67cRoom5) {
    s32 column = Actor_GetById(8)->x >> 20;

    if (trigger == 77 && column == 17)
      clear = 0;
    if (trigger == 78 && column == 19)
      clear = 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67cRoom6) {
    if (trigger == 77 && Actor_GetById(10)->x >> 20 == 9)
      clear = 0;
    if (trigger == 78 && Actor_GetById(11)->x >> 20 == 13)
      clear = 0;
  }
  if (clear == 0)
    return;
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  Func_02003d7c();
  Func_02003eac(0);
  goto test;
step:
  if (result < 0) {
    object->angle = 0x4000;
    Func_02000ca8(trigger);
    return;
  }
  {
    s32 offset, x;

    object->z += 0x20000;
    x = object->x;
    offset = (x & 0xfffff) - 0x80000;
    {
      /* FAKEMATCH(temporary): each bound in one local just past the limit; as
       * literal bounds, 4 of 120 instructions differ in tla-en. */
      s32 limit = 0x10001;

      if (offset >= limit)
        offset = 0x10000;
      limit = -0x10001;
      if (offset <= limit)
        offset = -0x10000;
    }
    object->x = x - offset;
  }
  object->angle += 0x1000;
  wait(1);
test:
  target.x = object->x;
  target.y = object->y;
  target.z = object->z + 0x80000;
  result = Func_02003d2c(object, &target);
  if (result <= 0)
    goto step;
}
