/// @file
/// Scene code of overlay 0x6a8, in ROM order, from s29a4's offset up to the next object the overlay links.

#include "types.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gMainRoutineWork[];

void Func_020029a4(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532)));
  FieldActor_SetSpritePriority(
      16, ((u32)(FIELD_AT(FIELD_AT(t0, s32, 80), u8, 9) << 28) >> 30));
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020045dc();
s32 Func_02001cf0();
extern u8 gMainRoutineWork[];

void Func_020029c8(void) {
  s32 t2;

  Func_020045dc(0);
  Task_AddFrame((s32)Func_020029a4, 0x480);
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x14) {
    t2 = Event_GetFlag(0x9c5);
    if (t2 == 0) {
      Func_02001cf0();
    }
  }
}
