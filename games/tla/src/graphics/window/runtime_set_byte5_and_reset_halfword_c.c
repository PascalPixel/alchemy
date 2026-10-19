/// @file
/// Display: change a sprite output's animation.

#include "types.h"
#include "render_input.h"

/// Set active to mode and reset the animation step (animStep) to zero.
/// out: sprite output; do nothing if NULL.
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode) {
  if (out != NULL) {
    out->active = mode;
    out->animStep = 0;
  }
}
