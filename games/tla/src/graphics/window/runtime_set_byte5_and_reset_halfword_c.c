/// @file
/// Display: change a sprite output's animation.

#include "types.h"
#include "render_input.h"

/// Sets active to mode and resets the animation step (animStep) to zero.
/// out: sprite output; nothing happens if NULL.
void WindowSprite_SetMotion(struct WindowSprite *out, s32 mode) {
  if (out != NULL) {
    out->active = mode;
    out->animStep = 0;
  }
}
