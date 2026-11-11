#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u32 gFrameCount;

u32 Random16Far(void);

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which sets the
   * arguments up in their own order: z is shifted before the -1 starts; as
   * direct Camera_MoveWithinMapBounds calls, 6 of 66 instructions differ in
   * tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
void Func_02001364(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16Far() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      Camera_MoveWithinMapBounds((x << 16) + 0x10000, -1, (z << 16) - 0x10000,
                                 1);
      break;
    }
    case 2:
      CameraMove((x << 16) + 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 3:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) - 0x10000, 1);
      break;
    }
  }
}
