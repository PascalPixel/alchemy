/// Releases the ride's handle.
void Ride_ReleaseHandle(void) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);

  Actor_Release(work->handle);
  work->handle = NULL;
}
