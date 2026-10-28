// Draft of Func_02000fd0 (overlay 690, 492 bytes), differ 35 of 199, registers
// only: the native keeps the object in r7, the column in r6 and the row in r5,
// and holds the address of the motion flags in r5 across the last wait; this
// form has the object in r5, the column in r7 and the row in r6. In the
// allocator the object ranks 9 uses over 32 instructions (its two uses in the
// sinking loop count double), the column 4 over 12 and the row 4 over 11; the
// native order needs the object below both. Measured: a for loop, the same
// registers; direct calls for the two speeds and the 0x207 flag share their
// constants in r5, r6 and r10 (differ 97); a flags pointer for the last block
// changes nothing.
#include "../../../../../games/tla/src/scene/settlements/local.h"

void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Func_0200156c(s32 id);

static __inline__ void Speed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
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
    /* FAKEMATCH: the loop is written with goto so that it carries no loop
     * notes: as a for loop its two uses of the object count double and the
     * object takes the row's register (differ 97 with the direct calls below).
     */
    i = 8;
  sink:
    FieldEvent_WaitFrames(1);
    object->y -= 0x20000;
    i--;
    if (i > 0)
      goto sink;
    Func_0200156c(10);
    FieldEvent_WaitFrames(20);
    Camera_MoveWithinMapBounds(0xc00000, -1, 0xe00000, 1);
    Speed(10, 0xcccc, 0x6666);
    Actor_OffsetPositionAndResetMotion(10, 0, 8);
    Actor_WaitForMoveById(10);
    FieldEvent_WaitFrames(10);
    Actor_ArmCallback(gAdventure.currentOwner, 0x4000, 0);
    Speed(10, 0x26666, 0x13333);
    Actor_OffsetPositionAndResetMotion(10, 0, 80);
    Actor_WaitForMoveById(10);
    Func_0200156c(10);
    if (!GameFlag_Test(0x204)) {
      FieldEvent_WaitFrames(10);
      Speed(10, 0xcccc, 0x6666);
      Actor_OffsetPositionAndResetMotion(10, 0, 80);
      Actor_WaitForMoveById(10);
      Speed(10, 0x28000, 0x14000);
      Actor_OffsetPositionAndResetMotion(10, 0, 80);
      FieldEvent_WaitFrames(5);
      Func_020020c0(10, 0, 0);
      GameFlag_SetBit(0x206);
    } else {
      Func_02002018(12, 32, 11, 15, 1, 1);
      Func_02002018(77, 15, 75, 15, 1, 1);
      Actor_OffsetPositionAndResetMotion(10, 0, 12);
      Actor_WaitForMoveById(10);
      FieldEvent_WaitFrames(20);
      Func_02002030(9, 11, 1, 1, 11, 11);
      Actor_SetModeById(10, 2);
      FieldEvent_WaitFrames(30);
      GameFlag_SetBit(0x205);
    }
    Func_02002070();
  } else if (!Flag_Test(0x207) && column == 16 && row == 12) {
    Func_02002030(17, 12, 1, 1, column, row);
    flags = &object->motionFlags;
    *flags = 3;
    FieldEvent_WaitFrames(30);
    *flags = 0;
    Flag_Set(0x207);
    Func_02002070();
  }
}
