/// @file
/// Fuchin Falls and Vale's cave: the logs that float the leader across a pool.

/// Floats the log actor id and the leader standing on it by (dx, dz) pixels,
/// to the centres of their cells, at a pixel a frame. The leader is drawn in
/// ANIM_LEADER_WALK_BACKWARDS and the log in its sheet's animation 4 going
/// north or west, 3 going south or east.
static void Actor_FloatLogWithLeader(s32 id, s32 dx, s32 dz) {
  struct Actor *leader;
  struct Actor *raft;
  FIXED x;
  FIXED z;

  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  raft = Actor_GetById(id);
  Event_Start();
  x = ((leader->x.fixed + INTtoFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((leader->z.fixed + INTtoFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

  leader->speedLimit = toFIXED(1);
  leader->acceleration = toFIXED(1) / 2;
  FieldActor_SetMoveTargetFX(leader, x, leader->y.fixed, z);
  Actor_SelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
  x = ((raft->x.fixed + INTtoFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((raft->z.fixed + INTtoFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);

  raft->speedLimit = toFIXED(1);
  raft->acceleration = toFIXED(1) / 2;
  FieldActor_SetMoveTargetFX(raft, x, raft->y.fixed, z);
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(raft, 4);
  } else {
    Actor_SelectAnimation(raft, 3);
  }
  Actor_WaitForMove(leader);
  Event_End();
}
