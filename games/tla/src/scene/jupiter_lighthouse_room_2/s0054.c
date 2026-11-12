/// @file
/// Scene code of overlay 0x69b, in ROM order, from s0054's offset up to the next object the overlay links.

#include "types.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gMainRoutineWork[];

void Func_02000054(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532)));
  if (FIELD_AT((t0 + 0x22), u8, 0) == 1) {
    MapGrid_SetCellBehavior(0x900000, 0x2400000, 2, 0xea);
    MapGrid_SetCellBehavior(0x1000000, 0x2300000, 2, 0xea);
  } else {
    MapGrid_SetCellBehavior(0x900000, 0x2400000, 2, 0xeb);
    MapGrid_SetCellBehavior(0x1000000, 0x2300000, 2, 0xeb);
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_0200231c();
extern u8 gData_02002b88[];
extern u8 gData_02002b60[];

void Func_020000b4(void) {
  Func_0200231c((s32)gData_02002b60, FIELD_AT(gData_02002b88, s32, 0),
                FIELD_AT(gData_02002b88, s32, 4),
                FIELD_AT(gData_02002b88, s32, 8));
}
