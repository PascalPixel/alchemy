#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02003398();
s32 Func_020033a0();
extern u8 gMainRoutineWork[];

void Func_02001660(void) {
  Func_02003398();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(FIELD_AT(gMainRoutineWork, s32, 532), 536, 360);
  Event_Wait(8);
  MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Sound_Play(0x248);
  Event_Wait(30);
  Sound_Play(0xbc);
  Map_CopyCellPictureIndicesRect(50, 20, 31, 20, 1, 2);
  Event_Wait(5);
  Map_CopyCellPictureIndicesRect(51, 20, 31, 20, 1, 2);
  Event_Wait(5);
  GameFlag_SetBit(0x200);
  Func_020033a0();
}
