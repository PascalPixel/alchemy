#include "types.h"
#include "event_inline.h"

void Func_02001fb0(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
  Event_SetFlag(0x250);
}

void Func_02001fd4(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
  Event_SetFlag(0x251);
}

void Func_02001ffc(void) {
  MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
  Event_SetFlag(0x252);
}
