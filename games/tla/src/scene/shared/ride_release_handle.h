/// Releases the ride's handle.
void Ride_ReleaseHandle(void) {
  struct FlameHitWork *work =
      (struct FlameHitWork *)(gInventorySnapshot + 0x188);

  Actor_Release(work->handle);
  work->handle = NULL;
}
