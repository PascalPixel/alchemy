/// @file
/// Scene code of overlay 0x682, in ROM order, from s0b18's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_020029fc();
s32 Func_02002ba4();
s32 Func_02002bac();
s32 Actor_SetActionCallback();
s32 Func_0200060c();
extern u8 Func_02000a80[];

s32 Func_02000b18(void) {
  s32 t7;
  s32 t8;
  s32 t12;
  s32 t19;
  s32 t23;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x100;
  Task_AddFrame((s32)Func_02000a80, 0xc80);
  Func_020029fc(8);
  Actor_SetSingleChildFlags(((s32)Actor_Get(10)), 0);
  Actor_SetSingleChildFlags(((s32)Actor_Get(11)), 0);
  FieldActor_SetSpritePriority(13, 3);
  t7 = ((s32)Actor_Get(23));
  FIELD_AT(t7, s32, 24) = 0x8000;
  t8 = ((s32)Actor_Get(23));
  FIELD_AT(t8, s32, 28) = 0x8000;
  Func_02002ba4();
  Func_02002bac(0, 8, 9);
  Func_020029fc(3);
  t12 = Event_GetFlag(0x97f);
  if (t12 != 0) {
    FieldActor_SetSpritePriority(20, 2);
    FieldActor_SetSpritePriority(21, 3);
    FieldActor_SetSpritePriority(22, 3);
    FieldActor_SetSpritePriority(19, 3);
    Actor_SetActionCallback(((s32)Actor_Get(18)), 9);
    t19 = Event_GetFlag(0x973);
    if (t19 != 0) {
      Actor_SetPosition(32, 0, 0);
      Actor_SetPosition(33, 0, 0);
      Actor_SetPosition(34, 0, 0);
    }
  }
  t23 = Event_GetFlag(0xa74);
  if (t23 != 0) {
    Func_0200060c();
  }
  return 0;
}

s32 Func_02000c04(void) {
  s32 t0;

  t0 = Event_GetFlag(0x97f);
  if (t0 != 0) {
    MapGrid_CopyCellsLayered(0xb, 0x33, 5, 4, 0x10, 0x17);
    MapGrid_CopyCellAttributes(11, 51, 5, 6, 16, 23);
  }
  return 0;
}
