/// Whether flame thrower `index` is active.
UNUSED s32 Ride_GetEntryValue(s32 index) {
  struct FlameHitState *state = RIDE_STATE;

  return (state->entries + index)->active;
}
