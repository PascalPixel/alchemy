/// Releases the flame whose picture the crumbs share (Ride_Create).
void Ride_ReleaseHandle(void) {
  struct FlameHitWork *work = RIDE_WORK;

  Actor_Release(work->handle);
  work->handle = NULL;
}
