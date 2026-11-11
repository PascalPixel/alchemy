#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02003380(s32 x, s32 z, s32 a, s32 b);
void Func_02003398(void);
void Func_020033a0(void);

static __inline__ void Func_02003328_(s32 srcX, s32 srcY, s32 dstX, s32 dstY,
                                      s32 width, s32 height) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which sets the
   * width before the height as the ROM allocates them (1 in r5, 2 in r6); as a
   * direct call, the two constants swap registers and 4 instructions differ. */
  Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
}

/// By the entrance taken, sets that entrance's flag, moves its actor to the
/// middle of its cell and copies a cell picture column twice beside it.
void Func_02001a94(void) {
  struct Actor *object;
  s32 column;
  s32 target[3];

  if (gMainRoutineWork.entrance == 1) {
    object = Actor_Get(8);
    Event_SetFlag(0x301);
    column = 11;
  } else if (gMainRoutineWork.entrance == 3) {
    object = Actor_Get(9);
    Event_SetFlag(0x302);
    column = 26;
  } else {
    object = Actor_Get(10);
    Event_SetFlag(0x303);
    column = 53;
  }
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  target[0] = (object->x & 0xfff00000) + 0x80000;
  target[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTarget(object, target[0], 0, target[2]);
  Actor_WaitForMove(object);
  Event_PlaySound(188);
  Func_02003328_(18, 4, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_CopyCellsTo(18, 7, column, 4, 1, 2);
  System_Wait(5);
  Func_02003380(column << 20, 0x580000, 0, 0);
  Func_020033a0();
}
