/// Checks the trap under the leader: the first step sets it off, the second
/// drops it.
void MagmaRockInside_CheckCrumblingStep(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Sound_Play(SE_CHEST_CLOSE);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(2);
      Actor_SelectAnimation(trap, 0);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      MagmaRockInside_SpawnRubble(0);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        leader->motionFlags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        MagmaRockInside_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(trap, 2);
        MagmaRockInside_SetLeaderCellShape(-2, trap->terrainId);
        MagmaRockInside_SpawnRubble(1);
        wait(16);
        MagmaRockInside_SetLeaderCellShape(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        MagmaRockInside_SpawnDustRing();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Event_End();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}
