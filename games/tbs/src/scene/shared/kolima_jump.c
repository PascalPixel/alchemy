/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance and SceneId_KolimaForestCliff):
/// the hop east onto the scene's third placed actor.

/// Hops the leader to the pixel of the 16.16 position request, unless something
/// blocks the way: the leader crouches, jumps (3 pixels a frame, rising at 4)
/// and lands while that actor plays its animation 7; the leader then
/// dips a pixel twice, its height and its ground height together, and rises
/// back. Returns 1 after a hop.
static s32 PushedActor_RunStepEffect(struct FieldPosition *request) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  u8 saved = actor->motionFlags;
  s32 ret = FieldActor_CheckMovementCollision(actor, request);

  if (ret == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(2);
    actor->velocityY = toFIXED(4);
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWaitById(
        ACTOR_PARTY_LEADER,
        ((struct FieldCoordinate *)&request->x)->part[0].integer,
        ((struct FieldCoordinate *)&request->z)->part[0].integer);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = ret;
    Actor_SetAnimation(ACTOR_FIRST_PLACED + 2, 7);
    actor->y.fixed -= toFIXED(1);
    actor->groundHeight -= toFIXED(1);
    wait(2);
    actor->y.fixed -= toFIXED(1);
    actor->groundHeight -= toFIXED(1);
    wait(10);
    actor->y.fixed += toFIXED(1);
    actor->groundHeight += toFIXED(1);
    wait(4);
    actor->y.fixed += toFIXED(1);
    actor->groundHeight += toFIXED(1);
    actor->motionFlags = saved;
    Event_End();
    return 1;
  }
  return 0;
}

/// Step trigger 11, walking east: hop 30 pixels east of the leader.
static void KolimaForest_HopEast(void) {
  struct FieldPosition target;
#if !TBS_PATCH_1
  target.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed + toFIXED(30);
  target.y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  target.z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed;
#else
  struct Actor *p = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 base = p->x.fixed & FIELD_CELL_MASK;

  target.x = base + toFIXED(8);
  target.y = p->y.fixed;
  target.z = (p->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  target.x = base + toFIXED(40);
#endif
  PushedActor_RunStepEffect(&target);
}
