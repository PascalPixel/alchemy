/// Runs MarsLighthouseRoomF_SlideOnIce while the ride is active; 1 when it ran.
s32 MarsLighthouseRoomF_ContinueSlide(void) {
  if (((struct RideWork *)(gInventorySnapshot + 0x188))->active) {
    MarsLighthouseRoomF_SlideOnIce();
    return 1;
  }
  return 0;
}
