#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02002d0c(s32 id);
void Func_02002d7c(void);
void Func_02003684(s32 actor);
extern const u8 gData_02004274[];

/// The room's setup: flags, the objects' places and the scene state.
void Func_02002f1c(void) {
  struct Actor *object;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 angle;
  s32 i;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  i = 11;
  do {
    Func_02002d0c(i++);
  } while (i <= 18);
  state = gSceneState;
  next = &gSceneState[1];
  if (!Event_GetFlag(0x109)) {
    *state = 0;
    *next = 0;
  }
  FieldTask_AddFrame(Func_02002d7c, 0x480);
  if (Event_GetFlag(0xa2c)) {
    object = Actor_Get(10);
    Actor_SetPosition(10, PIXELS(176), PIXELS(168));
    Actor_SetAnimation(10, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->y = PIXELS(-8);
    object->terrainHeight = PIXELS(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
  } else {
    Func_02003684(10);
  }
  if (!Event_GetFlag(0xaa2)) {
    FieldActor_WatchAndSetScript(9, 19);
    Actor_SetAnimation(19, 0);
    Actor_Get(19)->actionFlags &= ~1;
  } else if (!Event_GetFlag(0x109)) {
    other = Actor_Get(9);
    other->angle = FACING_SOUTH + FACING_STEP;
    other->collisionFlags |= 0x80;
    /* FAKEMATCH(temporary): the angle held in a local is built before the
     * position; as the constant argument 0x3000, 4 of 166 instructions differ
     * in tla-en. */
    angle = 0x3000;
    Actor_SetPositionAndFacing(19, PIXELS(234), PIXELS(204),
                                                   angle);
    Actor_Get(19)->collisionFlags |= 0x80;
    Actor_SetSpeed(9, 0x20000, 0x10000);
    Actor_SetSpeed(19, 0x19999, 0xcccc);
    Actor_SetScript(9, gData_02004274);
    Actor_SetScript(19, gData_02004274);
    Event_SetFlag(0x200);
  }
}

void Func_020030b4(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 3)
    Event_ClearFlag(0x12f);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  Actor_SetAnimation(10, 4);
  if (Event_GetFlag(0xaa3)) {
    spawned = FieldActor_Create(489, 0x680000, 0x180000, 0xa00000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Actor_SetItemIcon(spawned, 453);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->unk5c[0] = 1;
    }
    Actor_SetPosition(10, PIXELS(104), PIXELS(204));
  }
  FieldActor_SetSpritePriority(11, 1);
  Actor_SetSpriteColor(Actor_Get(11), 15);
  Actor_Get(11)->priorityFlags |= 2;
}
