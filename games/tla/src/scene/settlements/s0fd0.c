#include "../../../../../games/tla/src/scene/settlements/local.h"

void Func_0200156c(s32 id);

static __inline__ void Speed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which keeps
   * each call's own constants; as a direct Actor_SetSpeedParameters call it
   * does not. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

void Func_02000fd0(void) {
  struct Actor *object = Actor_GetById(10);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 i;
  u8 *flags;

  if (column == 11 && row == 9) {
    object->motionFlags = 0;
    // A goto loop carries no loop notes: as a for loop its two uses of the
    // object count double and the object takes the row's register.
    i = 8;
  sink:
    FieldEvent_Wait(1);
    object->y -= 0x20000;
    i--;
    if (i > 0)
      goto sink;
    Func_0200156c(10);
    FieldEvent_Wait(20);
    FieldCamera_MoveTo(0xc00000, -1, 0xe00000, 1);
    Speed(10, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldActor_WaitForMoveById(10);
    FieldEvent_Wait(10);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
    Speed(10, 0x26666, 0x13333);
    Actor_SetDestinationOffset(10, 0, 80);
    FieldActor_WaitForMoveById(10);
    Func_0200156c(10);
    if (!Event_GetFlag(0x204)) {
      FieldEvent_Wait(10);
      Speed(10, 0xcccc, 0x6666);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldActor_WaitForMoveById(10);
      Speed(10, 0x28000, 0x14000);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldEvent_Wait(5);
      Actor_SetPosition(10, 0, 0);
      Event_SetFlag(0x206);
    } else {
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
      Actor_SetDestinationOffset(10, 0, 12);
      FieldActor_WaitForMoveById(10);
      FieldEvent_Wait(20);
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
      Actor_SetAnimation(10, 2);
      FieldEvent_Wait(30);
      Event_SetFlag(0x205);
    }
    Func_02002070();
  } else if (!Flag_Test(0x207) && column == 16 && row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &object->motionFlags;
    *flags = 3;
    FieldEvent_Wait(30);
    *flags = 0;
    Flag_Set(0x207);
    Func_02002070();
  }
}
