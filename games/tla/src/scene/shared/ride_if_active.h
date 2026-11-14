/// Runs Fireball_CarryLeader while the ride is active; 1 when it ran.
s32 Fireball_CarryLeaderIfHit(void) {
  if (((struct FlameHitWork *)(gInventorySnapshot + 0x188))->active) {
    Fireball_CarryLeader();
    return 1;
  }
  return 0;
}
