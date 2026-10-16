#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern u32 gFrameCount;

struct ObjectRuntime *Object_GetById(s32 id);
u32 Random16Far(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order: the native shifts z before it starts the
   * -1; direct calls start the -1 first (differ 8, four reordered pairs). */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

// Every three frames, shake the camera diagonally by one pixel.
void Func_02001364(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16Far() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      s32 back = -0x10000;

      /* FAKEMATCH: this case calls directly, as its twin in ☀️ does, and the
       * barrier holds the -0x10000 in a register first: the native loads it
       * before it builds the 0x10000 of x; as one expression the load comes two
       * rows late (differ 2), and a loop note in the barrier's place changes
       * nothing. */
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
