#include "contigo.h"
#include "event_inline.h"

void Func_02001724(void) {
  MapGrid_CopyCellsTo(0, 1, 26, 42, 1, 2);
  Map_CopyCellPictureIndicesRect(0, 1, 26, 48, 1, 2);
  Map_CopyCellPictureIndicesRect(0, 1, 11, 54, 1, 2);
  Map_CopyCellPictureIndicesRect(0, 1, 16, 54, 1, 2);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 26, 45);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 26, 51);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 11, 57);
  Map_CopyCellAttributeRect(0, 32, 1, 1, 16, 57);
}
