#include "lemurian_ship.h"
#include "event_inline.h"

void Func_020027e4(void) {
  if (GameFlag_Test(0x983)) {
    Map_CopyCellPictureIndicesRect(17, 125, 27, 63, 3, 3);
    MapGrid_CopyCellAttributes(25, 0, 5, 3, 25, 4);
  }
}
