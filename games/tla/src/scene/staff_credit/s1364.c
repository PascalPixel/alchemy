#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

extern u32 gFrameCount;

struct Actor *Actor_GetById(s32 id);
u32 Random16Far(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order: the native shifts z before it starts the
   * -1; direct calls start the -1 first (differ 8, four reordered pairs). */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
void Func_02001364(void) {
  struct Actor *object = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16Far() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      s32 back = -0x10000;

      /* FAKEMATCH: the barrier loads -0x10000 before the 0x10000 of x is
       * built; as one expression, 2 instructions differ in tla-en. */
      __asm__("" : : "l"(back));
      Camera_MoveWithinMapBounds((x << 16) + 0x10000, -1, (z << 16) + back, 1);
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
