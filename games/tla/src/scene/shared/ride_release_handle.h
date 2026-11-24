/// Releases the flame whose picture the crumbs share (Ride_Create).
void Ride_ReleaseHandle(void) {
  struct FlameHitState *state = RIDE_STATE;

  Actor_Release(state->handle);
  state->handle = NULL;
}
