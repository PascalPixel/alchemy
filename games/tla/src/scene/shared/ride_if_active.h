/// Once a flame or a rolling fireball has hit the leader (the flames' work
/// active), it carries him along (Fireball_CarryLeader); 1 when it did.
s32 Fireball_CarryLeaderIfHit(void) {
  if ((RIDE_WORK)->active) {
    Fireball_CarryLeader();
    return 1;
  }
  return 0;
}
