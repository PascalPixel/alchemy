/// Turns flame thrower `index` on or off.
UNUSED void Ride_SetEntryValue(s32 index, s16 value) {
  struct FlameHitWork *work = RIDE_WORK;

  (work->entries + index)->active = value;
}
