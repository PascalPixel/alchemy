#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "scene_tables.h"
#include "event_inline.h"
s32 Func_02001380();
s32 Func_020013b8();
s32 Func_02001400();
s32 Func_02001338();
s32 Func_02001370();
s32 Func_020013f0();
s32 Func_02001388();

void Func_0200049c(s32 a0) {
  s32 t52;

  Func_02001380();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(30);
  Event_PlaySound(0x94);
  FieldEvent_Wait(100);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  FieldEvent_Wait(40);
  MapGrid_CopyCellsTo(0x52, 0x14, 0x46, 0, 3, 8);
  FieldEvent_Wait(3);
  MapGrid_CopyCellsTo(0x55, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x4f, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(70);
  Event_PlaySound(0x7e);
  if (a0 == -1) {
    t52 = Actor_Get(0x1d);
    Func_020013b8(0x1d, FIELD_AT(&gMainRoutineWork, s32, 532));
    FIELD_AT((t52 + 0x55), u8, 0) = 0;
    FIELD_AT(t52, s32, 12) = 0x200000;
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 0x1c);
    Func_02001400(0x1d);
    Func_02001338(0x16);
    Func_02001370(0x16);
    Actor_SetPosition(0x1d, 0, 0);
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  } else {
    Call2(Func_020013f0, a0, 3);
    Party_GiveItem(a0, 0);
    FieldEvent_Wait(20);
  }
  MapGrid_CopyCellsTo(0x61, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x4f, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  Func_02001388();
}
