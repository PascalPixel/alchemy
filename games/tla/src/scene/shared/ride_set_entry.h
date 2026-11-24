/// Sets flame thrower `index`'s flame (enum FlameThrowerFlags) and reach.
void Ride_SetEntry(s32 index, s32 value) {
  struct FlameHitState *state = RIDE_STATE;
  struct RideEntry *entry = &state->entries[index];

  entry->reach = INTtoFIXED(value & FLAME_THROWER_REACH_BITS);
  entry->flags = value & FLAME_THROWER_FLAG_BITS;
}
