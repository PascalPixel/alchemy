/// Unused: steps the flame throwers' counter while the party walks, wrapping
/// at `limit`, as the vents' Vent_AdvanceCounter does.
UNUSED s32 FlameThrowers_AdvanceCounter(s32 limit) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;

  if (!Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
    if (++state->counter >= limit)
      state->counter = 0;
  }
  return state->counter;
}
