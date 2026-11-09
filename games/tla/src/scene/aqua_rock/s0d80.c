#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/vector3.h"
#include "event_inline.h"

extern char SceneId_Ov67bRoom1;
extern u32 gFrameCount;
void Func_02001e00(void);
s32 Func_02002970(struct Actor *object, struct Vector3 *position);
void Func_02001d30(struct Actor *object);
void Func_02002910(struct Actor *object, s32 mode);
void Func_02002998(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
u32 Random16Far(void);
void Func_020029a0(void);

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

void Func_02000d80(void) {
  struct Actor *leader, *carried;
  struct Vector3 ahead;
  s32 owner;
  struct EventWork1cc *work;
  s32 result, i;

  if (GameFlag_Test(0xa68)) {
    work = Ram_HeapSlots->eventWork;
    owner = gMainRoutineWork.currentOwner;
    carried = work->object;
    leader = Actor_GetById(owner);
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
        leader->terrainHeight == 0)
      return;
    ahead.x = leader->x;
    ahead.y = leader->y;
    ahead.z = leader->z + 0x20000;
    result = Func_02002970(leader, &ahead);
    if ((gFrameCount & 4) == 0)
      Func_02001d30(leader);
    if (result < 0) {
      /* FAKEMATCH(wrapper): Call2 kept; as a direct call, 2 of 220 instructions
       * differ in tla-en. */
      Actor_SetAttachedEffect(owner, 258);
      Actor_SetMoveTarget(leader, leader->x, leader->y, leader->z - 0x80000);
      Func_02002910(leader, 7);
      Script_WaitForEventTimeout(leader);
      do
        wait(1);
      while (leader->y != leader->terrainHeight);
      Func_02001d30(leader);
      Func_02002910(leader, 6);
      wait(3);
      return;
    }
    ahead.x = leader->x;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x80000;
    result = Func_02002970(leader, &ahead);
    if (result > 0) {
      if (gMainRoutineWork.scene != (s32)&SceneId_Ov67bRoom1)
        return;
      Func_02002998();
      Event_ResolvePendingActions(0);
      leader->motionFlags = 0;
      Actor_SetSingleChildFlags(leader, 0);
      gMainRoutineWork.renderMode = 2;
      Func_02001d30(leader);
      Event_Wait(5);
      for (i = 59; i >= 0; i--) {
        leader->y += 0x20000;
        leader->angle += 0x1000;
        if (Random16Far() <= 0xfff)
          Func_02001d30(leader);
        wait(1);
      }
      gMainRoutineWork.renderMode = 0;
      Func_020029a0();
      Import_EventSetExitRequest(6);
      return;
    }
    ahead.x = leader->x + 0x5b333;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x5b333;
    result = Func_02002970(leader, &ahead);
    if (result > 0)
      return;
    ahead.x = leader->x - 0x5b333;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x5b333;
    result = Func_02002970(leader, &ahead);
    if (result > 0)
      return;
    carried->z += -0x20000;
    leader->z += -0x20000;
  } else
    Func_02001e00();
}
