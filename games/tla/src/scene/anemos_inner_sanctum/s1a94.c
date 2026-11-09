#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02003380(s32 x, s32 z, s32 a, s32 b);
void Func_02003398(void);
void Func_020033a0(void);

static __inline__ void Func_02003328_(s32 srcX, s32 srcY, s32 dstX, s32 dstY,
                                      s32 width, s32 height) {
  /* FAKEMATCH: forwarding through the inline sets the width before the height,
   * as the native allocates them (1 in r5, 2 in r6); called directly the two
   * constants swap registers (differ 4). */
  Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
}

/// By the entrance taken, sets that entrance's flag, moves its actor to the
/// middle of its cell and copies a cell picture column twice beside it.
void Func_02001a94(void) {
  struct Actor *object;
  s32 column;
  s32 target[3];

  if (gMainRoutineWork.entrance == 1) {
    object = Actor_GetById(8);
    GameFlag_SetBit(0x301);
    column = 11;
  } else if (gMainRoutineWork.entrance == 3) {
    object = Actor_GetById(9);
    GameFlag_SetBit(0x302);
    column = 26;
  } else {
    object = Actor_GetById(10);
    GameFlag_SetBit(0x303);
    column = 53;
  }
  Func_02003398();
  Event_ResolvePendingActions(0);
  target[0] = (object->x & 0xfff00000) + 0x80000;
  target[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003398();
  Event_ResolvePendingActions(0);
  Actor_SetMoveTarget(object, target[0], 0, target[2]);
  Script_WaitForEventTimeout(object);
  Sound_Play(188);
  Func_02003328_(18, 4, column, 4, 1, 2);
  wait(5);
  Map_CopyCellPictureIndicesRect(18, 7, column, 4, 1, 2);
  wait(5);
  Func_02003380(column << 20, 0x580000, 0, 0);
  Func_020033a0();
}
