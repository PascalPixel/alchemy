// Draft of Func_0200138c (overlay 690, 464 bytes), differ 50 of 196, registers
// only: the native keeps the object in r7, the column in r6 and the row in r5;
// this form has the object in r6 and the column in r7. The object is used in
// four sinking loops, and each use inside a loop counts double when the
// allocator ranks it, so it outranks the column. Measured: for loops, differ 66
// with the 0x205 flag shared; the loop as an inline function with a for loop,
// differ 94 (the object first of the three). The 0x205 pair goes through the
// flag forwarders so that each call builds its own constant.
#include "../../../../../games/tla/src/scene/settlements/local.h"

extern const u8 gData_0200232c[];

s32 Camera_FollowActor(s32 id, s32 a);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *callback);
void Func_02001620(s32 id);

/* FAKEMATCH: the sinking loop is written with goto so that it carries no loop
 * notes. */
#define SINK(frames, label)                                                    \
  i = (frames);                                                                \
  label:                                                                       \
  FieldEvent_WaitTicks(1);                                                    \
  object->y -= 0x20000;                                                        \
  i--;                                                                         \
  if (i > 0)                                                                   \
  goto label

void Func_0200138c(void) {
  struct ObjectRuntime *object;
  s32 column;
  s32 row;
  s32 i;

  Func_02002068();
  Func_02002150(0);
  Camera_FollowActor(10, 1);
  object = Object_GetById(10);
  column = object->x >> 20;
  row = object->z >> 20;
  if (column == 12 && row == 15) {
    GameFlag_SetBit(0x221);
    object->motionFlags = 0;
    SINK(32, sink1);
    Func_02001620(10);
    FieldEvent_WaitTicks(20);
  } else if (!Flag_Test(0x205) && column == 16 && row == 15) {
    object->motionFlags = 0;
    Flag_Set(0x205);
    GameFlag_SetBit(0x221);
    SINK(48, sink2);
    Func_02001620(10);
    FieldEvent_WaitTicks(30);
  } else if (column == 19 && row == 15) {
    Func_02002030(18, 16, 1, 1, column, row);
    object->motionFlags = 0;
    GameFlag_SetBit(0x221);
    SINK(32, sink3);
    Func_02001620(10);
    if (!GameFlag_Test(102)) {
      Func_020020c0(11, 0, 0);
      ObjectMotion_EnableActionAndSetCallback(10, gData_0200232c);
      FieldEvent_WaitTicks(20);
    }
    FieldEvent_WaitTicks(20);
  } else if (!GameFlag_Test(0x204) && column == 13 && row == 15) {
    Func_02002030(1, 3, 1, 1, 12, row);
    Func_02002030(1, 3, 1, 1, column, row);
    object->motionFlags = 0;
    SINK(16, sink4);
    Func_02001620(10);
    FieldEvent_WaitTicks(20);
    GameFlag_SetBit(0x204);
  } else if (column == 14 && row == 15) {
    Func_02002030(1, 3, 1, 1, 12, row);
    Func_02002030(1, 3, 1, 1, column, row);
  }
  Func_02002070();
}
