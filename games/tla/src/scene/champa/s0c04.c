#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

s32 Func_02000c04(void) {
  s32 t0;

  t0 = Event_GetFlag(0x97f);
  if (t0 != 0) {
    MapGrid_CopyCellsLayered(0xb, 0x33, 5, 4, 0x10, 0x17);
    MapGrid_CopyCellAttributes(11, 51, 5, 6, 16, 23);
  }
  return 0;
}
