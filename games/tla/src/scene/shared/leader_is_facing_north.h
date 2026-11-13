/// 1 when the leader faces north, within one eighth of a turn.
s32 Leader_IsFacingNorth(void) {
  u16 turn;
  s32 facing;

  turn = Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x5fff;
  facing = 1;
  if (turn >= 0x3fff)
    facing = 0;
  return facing;
}
