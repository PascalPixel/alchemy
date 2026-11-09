#include "contigo.h"
#include "event_inline.h"

void Func_0200152c(void) {
  MapGrid_CopyCellsTo(27, 4, 48, 12, 1, 2);
  Map_CopyCellPictureIndicesRect(27, 6, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 1, 2, 1, 48, 19);
}

void Func_0200156c(void) {
  MapGrid_CopyCellsTo(27, 8, 48, 12, 1, 2);
  Map_CopyCellPictureIndicesRect(27, 10, 49, 12, 1, 2);
  MapGrid_CopyCellAttributes(48, 0, 2, 1, 48, 19);
}
