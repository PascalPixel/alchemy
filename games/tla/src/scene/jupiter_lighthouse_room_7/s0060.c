/// @file
/// Scene code of overlay 0x6a0, in ROM order, from s0060's offset up to the next object the overlay links.

#include "jupiter_lighthouse_room_7.h"
#include "event_inline.h"
#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

void Func_02000060(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(81, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 41, 4, 1, 12, 35);
}

void Func_020000b4(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
  System_Wait(6);
  MapGrid_CopyCellsLayered(71, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 36, 4, 1, 12, 35);
}

void Func_02000100(void) {
  struct Actor *object = Actor_Get(16);

  if (!Event_GetFlag(0xa1b) && object->x >> 20 == 8 && object->z >> 20 == 37) {
    Event_SetFlag(0xa1b);
    Func_02000060(1);
  }
}

extern const u8 gData_020031c4[];
extern s32 gData_02003594;
extern s16 gData_020035fe;
extern struct Actor *gData_02003600[];
extern u16 gData_02003702;
void Func_02000060(s32 value);
void Func_020000b4(void);
void Func_02000100(void);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02001a48(const u8 *data, s32 a, s32 b);
void Func_02002bec(void);

void Func_0200013c(void) {
  if (!Event_GetFlag(514)) {
    Event_SetFlag(514);
    Func_02000060(1);
  }
}

void Func_02000160(void) {
  if (Event_GetFlag(514) == 1) {
    Event_ClearFlag(514);
    Func_020000b4();
  }
  Func_02000100();
}

void Func_02000184(void) {
  System_Wait(10);
  Func_02000100();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000194(void) {
  s32 t1, t2, t3, t5, t7, t9, t11, t13, v5, v6, v7, v8;

  t1 = ((s32)Actor_Get(18));
  v5 = t1;
  t2 = ((s32)Actor_Get(19));
  v7 = 1;
  v6 = (FIELD_AT(v5, s32, 8) >> 20);
  v8 = t2;
  v7 = -v7;
  v5 = (FIELD_AT(v5, s32, 16) >> 20);
  if (v6 != 35)
    goto L_020081d6;
  if (v5 != 15)
    goto L_020081d6;
  t3 = Event_GetFlag(0xa11);
  if (t3 != 0)
    goto L_020081d6;
  Event_SetFlag(0xa10);
  v7 = 0;
L_020081d6:
  if (v6 != 31)
    goto L_020081f8;
  if (v5 != 16)
    goto L_020081f8;
  t5 = Event_GetFlag(0xa13);
  if (t5 != 0)
    goto L_020081f8;
  Event_SetFlag(0xa12);
  v7 = 2;
L_020081f8:
  if (v6 != 33)
    goto L_0200821a;
  if (v5 != 10)
    goto L_0200821a;
  t7 = Event_GetFlag(0xa15);
  if (t7 != 0)
    goto L_0200821a;
  Event_SetFlag(0xa14);
  v7 = 4;
L_0200821a:
  v6 = (FIELD_AT(v8, s32, 8) >> 20);
  v5 = (FIELD_AT(v8, s32, 16) >> 20);
  if (v6 != 35)
    goto L_02008244;
  if (v5 != 15)
    goto L_02008244;
  t9 = Event_GetFlag(0xa10);
  if (t9 != 0)
    goto L_02008244;
  Event_SetFlag(0xa11);
  v7 = 1;
L_02008244:
  if (v6 != 31)
    goto L_02008266;
  if (v5 != 16)
    goto L_02008266;
  t11 = Event_GetFlag(0xa12);
  if (t11 != 0)
    goto L_02008266;
  Event_SetFlag(0xa13);
  v7 = 3;
L_02008266:
  if (v6 != 33)
    goto L_02008288;
  if (v5 != 10)
    goto L_02008288;
  t13 = Event_GetFlag(0xa14);
  if (t13 != 0)
    goto L_02008288;
  Event_SetFlag(0xa15);
  v7 = 5;
L_02008288:
  return v7;
}

s32 Func_02000290(void) {
  s32 count = 0;

  if (Event_GetFlag(0xa10) || Event_GetFlag(0xa12) || Event_GetFlag(0xa14))
    count = 1;
  if (Event_GetFlag(0xa11) || Event_GetFlag(0xa13) || Event_GetFlag(0xa15))
    count++;
  return count;
}

void Func_02002b54(s32 x, s32 z, s32 layer, s32 value);

/// Copies the map cells and sets the cell values that flags 0xa10 to 0xa15 call
/// for.
void Func_020002f0(void) {
  /* FAKEMATCH(wrapper): every call forwarded through the call layer; as direct
   * calls, 63 of 303 instructions differ in tla-en, the coordinates and flags
   * shared in saved registers. */
  MapGrid_CopyCellsLayered(0x5e, 0x3c, 9, 8, 0x5e, 0xa);
  Call4((void (*)())Func_02002b54, 0x2300000, 0xf00000, 0, -2);
  Call4((void (*)())Func_02002b54, 0x1f00000, 0x1000000, 0, -2);
  Call4((void (*)())Func_02002b54, 0x2100000, 0xa00000, 0, -2);
  MapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0);
  MapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0);
  MapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0);
  if (Event_GetFlag(0xa10) ||
      Event_GetFlag(0xa11))
    Func_02002b54(0x2300000, 0xf00000, 0, 0);
  if (Event_GetFlag(0xa12) ||
      Event_GetFlag(0xa13))
    Func_02002b54(0x1f00000, 0x1000000, 0, 0);
  if (Event_GetFlag(0xa14) ||
      Event_GetFlag(0xa15))
    Func_02002b54(0x2100000, 0xa00000, 0, 0);
  if (Event_GetFlag(0xa10)) {
    MapGrid_CopyCellsLayered(0x60, 0x38, 7, 2, 0x60, 0xf);
    MapGrid_CopyCellsLayered(0x6f, 0x33, 1, 1, 0x63, 0xf);
    if (Event_GetFlag(0xa13)) {
      MapGrid_CopyCellsLayered(0x69, 0x33, 1, 3, 0x5f, 0xe);
      MapGrid_SetCellBehavior(0x1f00000, 0xe00000, 2, 0xec);
    }
    if (Event_GetFlag(0xa15))
      MapGrid_CopyCellsLayered(0x6f, 0x35, 1, 1, 0x61, 0xa);
  } else if (Event_GetFlag(0xa11)) {
    MapGrid_CopyCellsLayered(0x62, 0x33, 5, 1, 0x62, 0xa);
    MapGrid_CopyCellsLayered(0x63, 0x34, 4, 5, 0x63, 0xb);
    MapGrid_SetCellBehavior(0x2600000, 0xa00000, 2, 0xec);
    if (Event_GetFlag(0xa12))
      MapGrid_CopyCellsLayered(0x6a, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa14)) {
      MapGrid_CopyCellsLayered(0x6d, 0x33, 3, 1, 0x5f, 0xa);
      MapGrid_SetCellBehavior(0x1f00000, 0xa00000, 2, 0xec);
    }
  } else {
    if (Event_GetFlag(0xa12))
      MapGrid_CopyCellsLayered(0x6a, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa13))
      MapGrid_CopyCellsLayered(0x6b, 0x35, 1, 1, 0x5f, 0x10);
    if (Event_GetFlag(0xa14))
      MapGrid_CopyCellsLayered(0x6f, 0x34, 1, 1, 0x61, 0xa);
    if (Event_GetFlag(0xa15))
      MapGrid_CopyCellsLayered(0x6f, 0x35, 1, 1, 0x61, 0xa);
  }
}
