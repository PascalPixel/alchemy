/// Felix, walking into one of Aqua Rock's streams, is carried down it two
/// pixels a frame, splashing, until something blocks the way; where it pours
/// off a ledge he drops with the water and lands sitting down. It runs every
/// frame he stands on the stream's cells; on the Moai room's dry ground it does
/// nothing.
void Leader_FlowWithStream(void) {
  struct Actor *camera;
  s32 id;
  struct Actor *leader;
  FIXED position[3];
  s32 hit;

  {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    id = gMainRoutineState.leaderActorId;
    camera = state->camera;
  }
  leader = Actor_Get(id);
  if (gMainRoutineState.next == SCENE_AQUA_ROCK_MOAI &&
      leader->groundHeight == 0)
    return;
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + toFIXED(2);
  hit = MapGrid_CheckCollision(leader, position);
  if ((gFrameCount & 4) == 0)
    Actor_SpawnWadingSplash(leader);
  if (hit < 0) {
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldActor_SetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
                               leader->z.fixed + toFIXED(8));
    Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
    FieldActor_WaitForMove(leader);
    do {
      System_Wait(1);
    } while (leader->y.fixed != leader->groundHeight);
    Actor_SpawnWadingSplash(leader);
    Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
    System_Wait(3);
    return;
  }
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + toFIXED(8);
  hit = MapGrid_CheckCollision(leader, position);
  if (hit > 0)
    return;
  position[0] = leader->x.fixed + (toFIXED(57) / 10);
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + (toFIXED(57) / 10);
  hit = MapGrid_CheckCollision(leader, position);
  if (hit > 0)
    return;
  position[0] = leader->x.fixed - (toFIXED(57) / 10);
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + (toFIXED(57) / 10);
  hit = MapGrid_CheckCollision(leader, position);
  if (hit > 0)
    return;
  camera->z.fixed += toFIXED(2);
  leader->z.fixed += toFIXED(2);
  leader->facing = FACING_SOUTH;
}
