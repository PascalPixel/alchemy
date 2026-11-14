
// Tolbi's ship deck and Vale's storm.

/// An actor script call, each frame: every 60 to 149 frames the actor
/// squashes and stretches back to its own shape over six frames.
static s32 Scene_SquashAndStretch(struct Actor *state) {
  switch (state->timer) {
  case 6:
    state->scaleX -= Q16_ONE / 4;
    state->scaleY += Q16_ONE / 8;
    break;
  case 4:
    state->scaleX += Q16_ONE / 8;
    state->scaleY -= Q16_ONE / 16;
    break;
  case 2:
    state->scaleX += Q16_ONE / 16;
    state->scaleY -= Q16_ONE / 32;
    break;
  case 0:
    state->scaleX = Q16_ONE;
    state->scaleY = Q16_ONE;
    state->timer = (Math_RemainderUnsigned(Random_Next(), 90) + 60);
    break;
  }
  state->timer--;
  return 1;
}
