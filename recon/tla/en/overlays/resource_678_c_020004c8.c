// Draft, differ 129 of 166: Taopo Swamp's sinking holes: looks up hole n (from
// 10) of the current room (Ov678Room5, 7, 8 or 9; Func_02002708) unless its
// flag says it is filled (0xa6e, 0xa6d, 545), then sinks the leader into
// it, slipping toward the hole's centre and turning, until Func_02002850
// reports the bottom; a blocked hole turns the leader round and runs
// Func_02000428. Left: the native keeps the hole number in r6 and the found result in r7 across the room tests, enters the sinking loop at its test and clamps the slip through two pool constants.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "system/vector3.h"

extern char SceneId_Ov678Room5, SceneId_Ov678Room7, SceneId_Ov678Room8,
    SceneId_Ov678Room9;
extern u8 gData_020029b8[], gData_020029c0[];
s32 Func_02002708(s32 hole);
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(u32 id);
void Func_02002898(void);
void Func_02002968(s32 mode);
s32 Func_02002850(struct Actor *object, struct Vector3 *position);
void Func_02000428(s32 hole);
void wait(s32 frames);

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
    found = Func_02002708((gData_020029c0 + 1)[hole - 10]);
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
  Func_02002968(0);
  for (;;) {
    position.x = leader->x;
    position.y = leader->y;
    position.z = leader->z + 0x80000;
    result = Func_02002850(leader, &position);
    if (result > 0)
      return;
    if (result < 0) {
      leader->angle = 0x4000;
      Func_02000428(hole);
      return;
    }
    leader->z += 0x20000;
    slip = (leader->x & 0xfffff) + 0xfff80000;
    if (slip > 0x10000)
      slip = 0x10000;
    if (slip < -0x10000)
      slip = -0x10000;
    leader->x -= slip;
    leader->angle += 0x1000;
    wait(1);
  }
}
