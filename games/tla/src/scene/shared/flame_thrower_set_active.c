/// Turns flame thrower `index` on or off.
UNUSED void FlameThrower_SetActive(s32 index, s16 value) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;

  (state->entries + index)->active = value;
}
