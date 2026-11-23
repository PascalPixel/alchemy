/// Stepping onto a crumbling pillar of an (actor, flag) list, as
/// CrumblingPillar_Step: the first time it flashes and cracks, the second time
/// it gives way under the leader with a shake, dust and a thump.
void Leader_CheckCrumblingStep(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  FIXED position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap == 0)
    return;
  while (*table != ACTOR_LIST_END) {
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      continue;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Sound_Play(SE_CHEST_CLOSE);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
      wait(2);
      Actor_SelectAnimation(trap, 0);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
      Dust_Scatter(0);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        leader->motionFlags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Dust_Scatter(0);
        wait(8);
        Actor_SelectAnimation(trap, 2);
        Leader_SetCellShapeAt(-2, trap->layer);
        Dust_Scatter(1);
        wait(16);
        Leader_SetCellShapeAt(-2, trap->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
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
}
