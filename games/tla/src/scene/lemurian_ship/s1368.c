#include "lemurian_ship.h"
#include "event_inline.h"
#include "version.h"

void Func_02001368(void) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  Event_ResolvePendingActions(0);
  Func_02002c70();
}

void Func_02001380(s32 id) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  ((struct LemurianShipObject *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->priorityFlags &= ~2;
  object->timer = 1;
}

// From patch 1; in the Japanese release actors 10 and 11 also use action 1.
#if TLA_PATCH_1
void Func_020013b8(s32 id) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  ((struct LemurianShipObject *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 2);
  object->priorityFlags &= ~2;
  object->timer = 1;
}
#endif

void Func_020013f0(void) {
  ((struct LemurianShipObject *)Actor_GetById(15))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(15, 3);
  ((struct LemurianShipObject *)Actor_GetById(15))->priorityFlags &= ~2;
  Func_02001380(16);
  Func_02001380(17);
  Func_02001380(18);
  Func_02001380(19);
  Func_02001380(20);
}

void Func_0200143c(void) {
  Func_02001380(16);
  Func_02001380(17);
  Func_02001380(18);
  Func_02001380(19);
  Func_02001380(20);
  Func_02001380(22);
}

void Func_02001464(void) {
#if !TLA_PATCH_1
  Func_02001380(10);
  Func_02001380(11);
#else
  Func_020013b8(10);
  Func_020013b8(11);
#endif
}

void Func_02001474(void) {
  struct LemurianShipObject *leader = ((
      struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner));
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  MapGrid_CopyCellsTo(74, 105, 4, 85, 8, 6);
  if (z == 25) {
    if (x == 4)
      MapGrid_CopyCellsTo(18, 126, 4, 85, 1, 2);
  } else if (z == 26) {
    if (x == 7)
      Map_CopyCellPictureIndicesRect(18, 126, 7, 86, 1, 1);
  } else if (z == 27) {
    if (x == 4)
      MapGrid_CopyCellsTo(18, 126, 4, 87, 1, 2);
    else if (x == 6)
      MapGrid_CopyCellsTo(18, 126, 6, 87, 1, 2);
    else if (x == 7)
      MapGrid_CopyCellsTo(18, 126, 7, 87, 1, 2);
    else if (x == 9)
      MapGrid_CopyCellsTo(18, 126, 9, 87, 1, 2);
  } else if (z == 29) {
    if (x == 6)
      MapGrid_CopyCellsTo(18, 126, 6, 89, 1, 2);
  }
}
