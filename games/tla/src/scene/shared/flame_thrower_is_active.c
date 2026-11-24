/// Whether flame thrower `index` is active.
UNUSED s32 FlameThrower_IsActive(s32 index) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;

  return (state->entries + index)->active;
}
