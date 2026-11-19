/// Flame thrower `index`'s flags: which flame it sends, or none
/// (FLAME_THROWER_DRY).
s32 Ride_GetEntryFlags(s32 index) {
  struct FlameHitWork *work = RIDE_WORK;

  return (work->entries + index)->flags;
}
