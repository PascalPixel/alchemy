/// Releases the ride's handle.
void Ride_ReleaseHandle(void) {
  struct FlameHitWork *work = RIDE_WORK;

  Actor_Release(work->handle);
  work->handle = NULL;
}
