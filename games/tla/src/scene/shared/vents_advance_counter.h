/// Steps the vents' counter, wrapping at `limit`.
s32 Vents_AdvanceCounter(s32 limit) {
  struct VentWork *work = VENT_WORK;

  if (Event_GetFlag(0x104) == 0) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}
