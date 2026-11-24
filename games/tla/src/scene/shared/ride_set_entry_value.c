/// Turns flame thrower `index` on or off.
UNUSED void Ride_SetEntryValue(s32 index, s16 value) {
  struct FlameHitState *state = RIDE_STATE;

  (state->entries + index)->active = value;
}
