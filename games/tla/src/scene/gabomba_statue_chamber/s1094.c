#include "gabomba_statue_chamber.h"
#include "event_inline.h"

void Func_02001094(void) {
  Map_CopyCellPictureIndicesRect(0, 1, 26, 6, 1, 1);
  Map_CopyCellPictureIndicesRect(0, 65, 26, 70, 1, 1);
  MapGrid_CopyCellAttributes(0, 2, 3, 3, 25, 5);
}
