/// Runs Fireball_CarryLeader while the ride is active; 1 when it ran.
s32 Fireball_CarryLeaderIfHit(void) {
  if ((RIDE_WORK)->active) {
    Fireball_CarryLeader();
    return 1;
  }
  return 0;
}
