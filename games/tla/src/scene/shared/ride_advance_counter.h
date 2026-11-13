/// Steps the ride's counter, wrapping at `limit`.
s32 Ride_AdvanceCounter(s32 limit) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);

  if (!Event_GetFlag(260)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}
