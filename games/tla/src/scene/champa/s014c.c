#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

void Func_0200014c(void) {
  s32 t2;
  s32 t1;

  TreasureSpot_SetPosition(0x40, 0x1880000, 0x2380000);
  t1 = ((s32)Actor_Get(64));
  FIELD_AT((t1 + 0x55), u8, 0) = 0;
  t2 = ((s32)Actor_Get(64));
  FIELD_AT(t2, s32, 12) = 0x200000;
}
