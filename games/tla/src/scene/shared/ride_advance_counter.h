/// Unused: steps the flame throwers' counter while the party walks, wrapping
/// at `limit`, as the vents' Vent_AdvanceCounter does.
UNUSED s32 Ride_AdvanceCounter(s32 limit) {
  struct FlameHitWork *work = RIDE_WORK;

  if (!Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}
