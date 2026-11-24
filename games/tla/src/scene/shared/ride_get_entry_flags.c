/// Flame thrower `index`'s flags: which flame it sends, or none
/// (FLAME_THROWER_DRY).
s32 Ride_GetEntryFlags(s32 index) {
  struct FlameHitState *state = RIDE_STATE;

  return (state->entries + index)->flags;
}
