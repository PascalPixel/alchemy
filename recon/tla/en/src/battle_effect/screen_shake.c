#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];
u32 Random16(void);

/// While shakeFrames lasts, moves BG1 and the projection centre up or down
/// by a random offset within range; then restores them.
void Camera_ApplyVerticalShake(s32 range) {
  u8 *fx = Ram_HeapSlots->battleFxWork[0];
  s32 *timer;

  timer = (s32 *)(fx + SPLIT_CONSTANT(0x77a8));
  if (*timer > 0) {
    s32 shake = ((range - 1) & Random16()) - range / 2;

    gBgScroll[1].y = shake + 32;
    gCameraSceneParameters[4] = 120 - shake;
    (*timer)--;
  } else {
    gBgScroll[1].y = *(s32 *)(fx + 0x77a4);
    gCameraSceneParameters[4] = 120;
  }
}
