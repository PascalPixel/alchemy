#include "local.h"

extern const u8 gData_0200232c[];

void Func_02001620(s32 id);

#define SINK(frames)                                                           \
  i = (frames);                                                                \
  do {                                                                         \
    FieldEvent_Wait(1);                                                        \
    object->y -= 0x20000;                                                      \
    i--;                                                                       \
  } while (i > 0)

void Func_0200138c(void) {
  struct Actor *object;
  s32 column;
  s32 row;
  s32 i;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_FollowActor(10, 1);
  object = Actor_Get(10);
  column = object->x >> 20;
  row = object->z >> 20;
  if (column == 12 && row == 15) {
    Event_SetFlag(0x221);
    object->motionFlags = 0;
    SINK(32);
    Func_02001620(10);
    FieldEvent_Wait(20);
  } else if (!Flag_Test(0x205) && column == 16 && row == 15) {
    object->motionFlags = 0;
    Flag_Set(0x205);
    Event_SetFlag(0x221);
    SINK(48);
    Func_02001620(10);
    FieldEvent_Wait(30);
  } else if (column == 19 && row == 15) {
    MapGrid_CopyCellAttributes(18, 16, 1, 1, column, row);
    object->motionFlags = 0;
    Event_SetFlag(0x221);
    SINK(32);
    Func_02001620(10);
    if (!Event_GetFlag(102)) {
      Actor_SetPosition(11, 0, 0);
      Actor_SetScript(10, gData_0200232c);
      FieldEvent_Wait(20);
    }
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(0x204) && column == 13 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
    object->motionFlags = 0;
    SINK(16);
    Func_02001620(10);
    FieldEvent_Wait(20);
    Event_SetFlag(0x204);
  } else if (column == 14 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
  }
  Func_02002070();
}
