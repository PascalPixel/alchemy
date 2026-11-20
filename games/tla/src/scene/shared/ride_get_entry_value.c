/// Whether flame thrower `index` is active.
UNUSED s32 Ride_GetEntryValue(s32 index) {
  struct FlameHitWork *work = RIDE_WORK;

  return (work->entries + index)->active;
}
