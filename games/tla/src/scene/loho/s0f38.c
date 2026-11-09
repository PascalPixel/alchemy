#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_020012e8(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020012a8_(s32 srcX, s32 srcY, s32 width, s32 height,
                                      s32 destX, s32 destY) {
  Map_CopyCellPictureIndicesRect(srcX, srcY, width, height, destX, destY);
}

static __inline__ void Func_02001348_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_02000f38(void) {
  s32 value = ((struct EventWork154 *)Ram_HeapSlots->eventWork)->value;

  if (value == 2)
    Func_02001348_(8, PIXELS(144), PIXELS(224));
  else
    Func_02001348_(8, PIXELS(224), PIXELS(224));
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= 0xfe;
  Func_020012a8_(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(14);
  Sound_Play(123);
  Import_EventSetExitRequest(value);
  Func_020012e8();
}
