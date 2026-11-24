/// Sets flame thrower `index`'s flame (enum FlameThrowerFlags) and reach.
void FlameThrower_Set(s32 index, s32 value) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;
  struct FlameThrower *entry = &state->entries[index];

  entry->reach = INTtoFIXED(value & FLAME_THROWER_REACH_BITS);
  entry->flags = value & FLAME_THROWER_FLAG_BITS;
}
