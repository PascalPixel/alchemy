// Matches: differ 0 of 223 (tla-en, 2026-10-30): the rooms' second setup.
// Task_Add now has its own declaration from task.h, returning s32: declared
// void, its call did not set r0, so the flag test's argument stayed
// output-dependent on the task routine's address load and sched2 issued that
// load before the mode store (differ 2). The terrain calls and the second
// flag test need no wrapper; the wrappers left are tagged below.
#include "../../../../games/tla/src/scene/alhafran_cave/local.h"
#include "call.h"
#include "task.h"

void Func_020000fc(void);
void Func_02000220(void);
void Func_02001488(s32 actor, s32 x, s32 z);
void Func_02001578(const void *data);
void Func_02001598(s32 actor, s32 value);
void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_020002f8(void) {
  struct AlhafranCaveMapWork *map =
      (struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object;

  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  map->mode17 = 1;
  Task_Add(Func_02000220, TASK_PASS_VBLANK);
  if (GameFlag_Test(0x204))
    Func_020000fc();
  object = Actor_GetById(8);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_GetById(9);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_GetById(10);
  object->motionFlags = 0;
  object->y = 0xc0000;
  Animation_ApplyChildValues(Actor_GetById(8), 6);
  Animation_ApplyChildValues(Actor_GetById(9), 6);
  Animation_ApplyChildValues(Actor_GetById(10), 6);
  object = Actor_GetById(12);
  object->motionFlags = 0;
  object->y = 0x100000;
  object = Actor_GetById(11);
  object->motionFlags = 0;
  object->y = 0x100000;
  /* FAKEMATCH(wrapper): the call forwarded through Call2; as a direct call,
   * 4 of 223 instructions differ in tla-en. */
  Call2((void (*)())Func_02001598, 13, 0x1000);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 3);
  Actor_SetModeById(13, 2);
  Actor_SetSpritePriority(11, 2);
  Actor_SetSpritePriority(12, 2);
  Actor_SetSpritePriority(13, 3);
  Actor_GetById(11)->priorityFlags |= 2;
  Actor_GetById(12)->priorityFlags |= 2;
  Actor_GetById(13)->priorityFlags |= 2;
  Animation_ApplyChildValues(Actor_GetById(11), 6);
  Animation_ApplyChildValues(Actor_GetById(12), 6);
  Animation_ApplyChildValues(Actor_GetById(13), 6);
  /* FAKEMATCH(wrapper): the flag test forwarded through Value1; as a direct
   * call, 11 of 223 instructions differ in tla-en. */
  if (Value1((s32 (*)())GameFlag_Test, 0x8a7)) {
    /* FAKEMATCH(wrapper): both calls forwarded through Call3; either as a
     * direct call, 2 of 223 instructions differ in tla-en. */
    Call3((void (*)())Func_02001488, 14, 0xc80000, 0x880000);
    Call3((void (*)())Func_02001488, 15, 0x1880000, 0x1680000);
  }
  object = Actor_GetById(14);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  Func_02001420(object->x, object->z, 0, 4);
  object = Actor_GetById(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  Func_02001420(object->x, object->z, 0, 4);
  Func_02001578(gData_02001608);
  Actor_SetActionCallback(Actor_GetById(16), 9);
  if (!GameFlag_Test(0x8a7)) {
    /* FAKEMATCH(wrapper): the three calls forwarded through Func_02001408_;
     * any one as a direct call, 4 of 223 instructions differ in tla-en. */
    Func_02001408_(0, 58, 12, 7, 13, 58);
    Func_02001408_(0, 58, 2, 9, 25, 66);
    Func_02001408_(0, 58, 10, 5, 15, 74);
  }
}
