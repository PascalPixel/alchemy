/// Flame thrower `index`'s flags: which flame it sends, or none
/// (FLAME_THROWER_DRY).
s32 FlameThrower_GetFlags(s32 index) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;

  return (state->entries + index)->flags;
}
