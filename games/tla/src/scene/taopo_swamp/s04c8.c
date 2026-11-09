#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "system/vector3.h"
#include "event_inline.h"

extern char SceneId_Ov678Room5, SceneId_Ov678Room7, SceneId_Ov678Room8,
    SceneId_Ov678Room9;
extern u8 gData_020029b8[];
extern u8 gData_020029c0[];
s32 Func_02002708(s32 hole);
void Func_02002898(void);
s32 Func_02002850(struct Actor *object, struct Vector3 *position);
void Func_02000428(s32 hole);

void Func_020004c8(s32 hole) {
  struct Actor *leader;
  struct Vector3 position;
  s32 found, result, slip;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room5) {
    found = Func_02002708(gData_020029b8[hole - 10]);
    if (hole == 12 && GameFlag_Test(0xa6e))
      return;
    if (hole == 18 && GameFlag_Test(0xa6d))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room7) {
    u8 *table = gData_020029c0 + 1;
    s32 at = hole - 10;
    found = Func_02002708(table[at]);
    if (hole == 10 && GameFlag_Test(545))
      return;
    if (hole == 12 && GameFlag_Test(545))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room8) {
    found = Func_02002708(hole - 10);
    if (hole == 10 && GameFlag_Test(545))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room9)
    found = Func_02002708(hole - 10);
  if (found == 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  Func_02002898();
  Event_ResolvePendingActions(0);
  goto test;
sink:
  if (result < 0) {
    leader->angle = 0x4000;
    Func_02000428(hole);
    return;
  }
  leader->z += 0x20000;
  slip = (leader->x & 0xfffff) + 0xfff80000;
  {
    /* FAKEMATCH(temporary): each bound in one local just past the limit; as literal
     * bounds, 73 of 166 instructions differ in tla-en. */
    s32 limit = 0x10001;

    if (slip >= limit)
      slip = 0x10000;
    limit = -0x10001;
    if (slip <= limit)
      slip = -0x10000;
  }
  leader->x -= slip;
  leader->angle += 0x1000;
  wait(1);
test:
  position.x = leader->x;
  position.y = leader->y;
  position.z = leader->z + 0x80000;
  result = Func_02002850(leader, &position);
  if (result <= 0)
    goto sink;
}
