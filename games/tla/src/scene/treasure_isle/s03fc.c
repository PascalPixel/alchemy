#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

void Func_020003fc(s32 a0, s32 a1) {
  MapGrid_CopyCellsLayered(0, 0x64, 3, 4,
                           (FIELD_AT((((a1 << 16) >> 12) + a0), s32, 0) + 0x3f),
                           (FIELD_AT((((a1 << 16) >> 12) + a0), s32, 4) - 2));
  MapGrid_CopyCellAttributes(1, 0, 1, 1,
                             FIELD_AT((((a1 << 16) >> 12) + a0), s32, 0),
                             FIELD_AT((((a1 << 16) >> 12) + a0), s32, 4));
  GameFlag_SetBit(FIELD_AT((((a1 << 16) >> 12) + a0), u16, 12));
}
