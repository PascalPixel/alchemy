/// Releases the flame whose picture the crumbs share (FlameThrowers_Create).
void FlameThrowers_ReleaseFlame(void) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;

  Actor_Release(state->handle);
  state->handle = NULL;
}
