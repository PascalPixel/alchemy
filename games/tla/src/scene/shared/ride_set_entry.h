/// A flame thrower's packed state: the flame's reach in the low four bits,
/// enum FlameThrowerFlags in the next four.
#define FLAME_THROWER_REACH_BITS 0xf
#define FLAME_THROWER_FLAG_BITS 0xf0

/// Sets flame thrower `index`'s flame (enum FlameThrowerFlags) and reach.
void Ride_SetEntry(s32 index, s32 value) {
  struct FlameHitWork *work = RIDE_WORK;
  struct RideEntry *entry = &work->entries[index];

  entry->reach = INTtoFIXED(value & FLAME_THROWER_REACH_BITS);
  entry->flags = value & FLAME_THROWER_FLAG_BITS;
}
