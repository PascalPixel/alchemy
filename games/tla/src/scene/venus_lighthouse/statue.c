#include "types.h"
#include "field_event.h"

void SceneState_ApplySixRectsAfter161(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  Import_MapCopyCellAttributeRect(35, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  Import_MapCopyCellPictureIndicesRect(35, 8, 23, 8, b, a);
  Import_MapCopyCellPictureIndicesRect(99, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  Import_MapCopyCellAttributeRect(57, 55, 3, 3, x, y);
  Import_MapCopyCellPictureIndicesRect(57, 55, 46, 55, a, a);
  Import_MapCopyCellPictureIndicesRect(121, 55, 110, 55, a, a);
}

void SceneState_ApplySixRectsAfterFlag161(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  Import_MapCopyCellAttributeRect(36, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  Import_MapCopyCellPictureIndicesRect(36, 8, 23, 8, b, a);
  Import_MapCopyCellPictureIndicesRect(100, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  Import_MapCopyCellAttributeRect(53, 55, 3, 3, x, y);
  Import_MapCopyCellPictureIndicesRect(53, 55, 46, 55, a, a);
  Import_MapCopyCellPictureIndicesRect(117, 55, 110, 55, a, a);
}
