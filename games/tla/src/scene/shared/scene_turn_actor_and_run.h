/// Turns actor 4, then runs Mikasalla_ClimbDownLadder.
void Mikasalla_TurnAndClimbDownLadder(void) {
  s32 id, a;

  Event_Start();
  Event_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm. */
  __asm__ volatile("" : "+r"(id));
  Actor_ArmCallback(id, a << 8, 0);
  Mikasalla_ClimbDownLadder();
}
