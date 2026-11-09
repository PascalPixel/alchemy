#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

void Func_02002878(void) {
  s32 t9;
  s32 t3;
  s32 t4;
  s32 t6;
  s32 t8;
  s32 t11;
  s32 t16;
  s32 t20;

  FieldActor_SetSpritePriority(19, 1);
  Actor_SetSpriteColor(((s32)Actor_Get(19)), 0xf);
  t3 = Event_GetFlag(0xa3d);
  if (t3 == 0) {
    t4 = Event_GetFlag(0x9c0);
    if (t4 == 0) {
      Actor_SetPosition(8, 0, 0);
    }
    t6 = Event_GetFlag(0x200);
    if (t6 != 0) {
      MapGrid_CopyCellsTo(0x59, 0, 0x4d, 0x10, 1, 2);
    }
    t8 = Event_GetFlag(0x9c1);
    if (t8 != 0) {
      t9 = ((s32)Actor_Get(10));
      FIELD_AT(t9, u16, 6) = 0xb000;
      System_Wait(1);
    }
  }
  t11 = Event_GetFlag(0xa3d);
  if (t11 != 0) {
    MapGrid_CopyCellsTo(0x48, 3, 0x45, 3, 3, 3);
    MapGrid_CopyCellsTo(0x30, 0x24, 0x45, 8, 3, 2);
    MapGrid_CopyCellAttributes(0x48, 0xd, 1, 3, 0x46, 0x14);
    Actor_SetPosition(20, 0, 0);
  } else {
    t16 = Event_GetFlag(0xa2d);
    if (t16 != 0) {
      MapGrid_CopyCellsTo(0x48, 3, 0x45, 3, 3, 3);
      MapGrid_CopyCellsTo(0x30, 0x24, 0x45, 8, 3, 2);
      MapGrid_CopyCellAttributes(0x48, 0xd, 1, 3, 0x46, 0x14);
      t20 = Event_GetFlag(0x40);
      if (t20 == 0) {
        Actor_SetPosition(20, 0, 0);
      }
    }
  }
}
