#include "gabomba_statue_chamber.h"
#include "event_inline.h"

void Func_02001010(void) {
  Map_CopyCellPictureIndicesRect(85, 78, 22, 79, 5, 11);
  MapGrid_CopyCellAttributes(85, 78, 5, 11, 22, 15);
  MapGrid_CopyCellsTo(86, 30, 86, 20, 5, 2);
  Map_CopyCellPictureIndicesRect(86, 30, 84, 22, 9, 2);
}
