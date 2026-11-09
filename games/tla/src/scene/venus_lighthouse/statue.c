#include "types.h"
#include "field_event.h"
#include "event_inline.h"

void SceneState_ApplySixRectsAfter161(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(35, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  MapGrid_CopyCellsTo(35, 8, 23, 8, b, a);
  MapGrid_CopyCellsTo(99, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(57, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(57, 55, 46, 55, a, a);
  MapGrid_CopyCellsTo(121, 55, 110, 55, a, a);
}

void SceneState_ApplySixRectsAfterFlag161(void) {
  s32 x;
  s32 y;
  s32 a;
  s32 b;

  x = 23;
  y = 8;
  MapGrid_CopyCellAttributes(36, 8, 1, 3, x, y);
  a = 3;
  b = 1;
  MapGrid_CopyCellsTo(36, 8, 23, 8, b, a);
  MapGrid_CopyCellsTo(100, 8, 87, 8, b, a);
  x = 46;
  y = 55;
  MapGrid_CopyCellAttributes(53, 55, 3, 3, x, y);
  MapGrid_CopyCellsTo(53, 55, 46, 55, a, a);
  MapGrid_CopyCellsTo(117, 55, 110, 55, a, a);
}
