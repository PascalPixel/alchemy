#include "../../../../games/tla/src/scene/alhafran_cave/local.h"
#include "task.h"

void Func_020000fc(void);
void Func_02000220(void);
void Func_02001578(const void *data);
void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_020002f8(void) {
  struct AlhafranCaveMapWork *map =
      (struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  map->mode17 = 1;
  Task_AddFrame(Func_02000220, TASK_PASS_VBLANK);
  if (Event_GetFlag(0x204))
    Func_020000fc();
  object = Actor_Get(8);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_Get(9);
  object->motionFlags = 0;
  object->y = 0xc0000;
  object = Actor_Get(10);
  object->motionFlags = 0;
  object->y = 0xc0000;
  Animation_ApplyChildValues(Actor_Get(8), 6);
  Animation_ApplyChildValues(Actor_Get(9), 6);
  Animation_ApplyChildValues(Actor_Get(10), 6);
  object = Actor_Get(12);
  object->motionFlags = 0;
  object->y = 0x100000;
  object = Actor_Get(11);
  object->motionFlags = 0;
  object->y = 0x100000;
  /* FAKEMATCH(wrapper): the call forwarded through Call2; as a direct call,
   * 4 of 223 instructions differ in tla-en. */
  Actor_SetSpriteRotationById(13, 0x1000);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 3);
  Actor_SetAnimation(13, 2);
  FieldActor_SetSpritePriority(11, 2);
  FieldActor_SetSpritePriority(12, 2);
  FieldActor_SetSpritePriority(13, 3);
  Actor_Get(11)->priorityFlags |= 2;
  Actor_Get(12)->priorityFlags |= 2;
  Actor_Get(13)->priorityFlags |= 2;
  Animation_ApplyChildValues(Actor_Get(11), 6);
  Animation_ApplyChildValues(Actor_Get(12), 6);
  Animation_ApplyChildValues(Actor_Get(13), 6);
  /* FAKEMATCH(wrapper): the flag test forwarded through Value1; as a direct
   * call, 11 of 223 instructions differ in tla-en. */
  if (Event_GetFlag(0x8a7)) {
    /* FAKEMATCH(wrapper): both calls forwarded through Call3; as either one a
     * direct call, 2 of 223 instructions differ in tla-en. */
    Actor_SetPosition(14, 0xc80000, 0x880000);
    Actor_SetPosition(15, 0x1880000, 0x1680000);
  }
  object = Actor_Get(14);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  Func_02001420(object->x, object->z, 0, 4);
  object = Actor_Get(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  Func_02001420(object->x, object->z, 0, 4);
  Func_02001578(gData_02001608);
  Actor_SetActionCallback(Actor_Get(16), 9);
  if (!Event_GetFlag(0x8a7)) {
    /* FAKEMATCH(wrapper): the three calls forwarded through Func_02001408_; as
     * any one a direct call, 4 of 223 instructions differ in tla-en. */
    Func_02001408_(0, 58, 12, 7, 13, 58);
    Func_02001408_(0, 58, 2, 9, 25, 66);
    Func_02001408_(0, 58, 10, 5, 15, 74);
  }
}
